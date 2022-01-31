/**
 * @file model.cpp
 * @author Matthew Richarson
 * @brief Define Model and Mesh classes for rendering and importing geometry form external file formats.
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/model.hpp"

#include "headers/standard-functions.hpp"
#include "headers/camera.hpp"
#include <glad/glad.h>

#include <iostream>

std::vector<Texture> g_loadedTextures;

/**
 * @brief Pass in and set inital data for Mesh class. Then initialise instance.
 * 
 * @param vertices 
 * @param indices 
 * @param textures 
 */
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	initMesh();
};

/**
 * @brief Initialise instance.
 * 
 */
void Mesh::initMesh() {
	// Gen VAO, VBO and EBO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);


	// Set location of different coords within VAO
	// Verts
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	glEnableVertexAttribArray(1);

	// Textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
	glEnableVertexAttribArray(2);

	// Tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);


	// Unbind VAO
	glBindVertexArray(0);

}

/**
 * @brief Draw mesh.
 * 
 * @param shaderProgram 
 * @param camera 
 * @param worldPos 
 */
void Mesh::draw(unsigned int shaderProgram, Camera camera, glm::mat4 worldPos, std::vector<Light> sceneLights) {
	// Use shader program
	glUseProgram(shaderProgram);

	// Textures
	unsigned int texturesCount = 0;
	unsigned int diffusionCount = 0; unsigned int specularCount = 0; unsigned int normalCount = 0;
	for (int i = 0; i < m_textures.size(); i++) {
		if (m_textures[i].type == "diffuse") {
			diffusionCount++; texturesCount = diffusionCount;
		}
		else if (m_textures[i].type == "specular") {
			specularCount++; texturesCount = specularCount;
		}
		else {
			normalCount++; texturesCount = normalCount;
		}

		glUniform1i(glGetUniformLocation(shaderProgram, (m_textures[i].type + std::to_string(texturesCount)).c_str()), i);
		glActiveTexture(GL_TEXTURE0+i); glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		texturesCount = 0;
	}

	// Does the model have a diffusion/spec/normal map
	glUniform1i(glGetUniformLocation(shaderProgram, "diffusionMapCount"), diffusionCount);
	glUniform1i(glGetUniformLocation(shaderProgram, "specularMapCount"), specularCount);
	glUniform1i(glGetUniformLocation(shaderProgram, "normalMapCount"), normalCount);

	// Lighting
	for (int i=0; i<sceneLights.size(); i++) {
		std::string posName = "sceneLights[" + std::to_string(i) + "].position";
		std::string colorName = "sceneLights[" + std::to_string(i) + "].color";
		std::string typeName = "sceneLights[" + std::to_string(i) + "].type";

		glUniform3fv(glGetUniformLocation(shaderProgram, posName.c_str()), 1, glm::value_ptr(sceneLights[i].location));
		glUniform3fv(glGetUniformLocation(shaderProgram, colorName.c_str()), 1, glm::value_ptr(sceneLights[i].color));
		glUniform1i(glGetUniformLocation(shaderProgram, typeName.c_str()), sceneLights[i].type);
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "lightsCount"), sceneLights.size());

	// Set uniforms
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "world"), 1, GL_FALSE, glm::value_ptr(worldPos));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.m_pos));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera.m_projection));

	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(camera.m_loc));

	// Bind VAO, draw
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

/**
 * @brief Load model file and initalise submeshes.
 * 
 * @param path 
 * @param spawnLocation 
 */
Model::Model(std::string path, glm::vec3 spawnLocation) {
	load_model(path);

	m_worldPos = glm::mat4(1.0);
	set_loc(spawnLocation);
}

/**
 * @brief Load model file.
 * 
 * @param path 
 */
void Model::load_model(std::string path) {
	// Import file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// Print any errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	// Get directory for loading textures etc
	m_directory = path.substr(0, path.find_last_of("/"));

	// Process root node
	process_node(scene->mRootNode, scene);
}

/**
 * @brief Process node of imported model file.
 * 
 * @param node 
 * @param scene 
 */
void Model::process_node(aiNode* node, const aiScene* scene) {
	// Process meshes
	for (int i = 0; i < node->mNumMeshes; i++) {
		// Add processed mesh (indexed by the node) to the list of model meshes
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(process_mesh(mesh, scene));
	}

	// Do the same for children
	for (int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

/**
 * @brief Initialize mesh to match node of assimp import tree.
 * 
 * @param mesh 
 * @param scene 
 * @return Mesh 
 */
Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
	// Get verticies
	std::vector<Vertex> verts;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vert;

		// Get vertex pos
		vert.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		// Get vertex normals
		vert.norm = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		// Get texture coords
		glm::vec2 texCoords = glm::vec2(0.0);
		if (mesh->mTextureCoords[0] != NULL) {
			texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		vert.tex = texCoords;

		// Get tangents
		vert.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

		verts.push_back(vert);
	}

	// Get indicies
	std::vector<unsigned int> indices;

	for (int i = 0; i < mesh->mNumFaces; i++) {
		// Get faces (individual primitives)
		aiFace face = mesh->mFaces[i];
		
		// Get indicies for every face
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Get materials
	std::vector<Texture> textures;
	std::vector<Texture> diffuseMaps; std::vector<Texture> specularMaps; std::vector<Texture> normalMaps;
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		diffuseMaps = load_textures(mat, aiTextureType_DIFFUSE, "diffuse");
		specularMaps = load_textures(mat, aiTextureType_SPECULAR, "specular");
		// obj files use "height" not "normals"
		normalMaps = load_textures(mat, aiTextureType_HEIGHT, "normal");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(verts, indices, textures);
}

/**
 * @brief Load all required textures from assimp import.
 * 
 * @param mat 
 * @param type 
 * @param typeName 
 * @return std::vector<Texture> 
 */
std::vector<Texture> Model::load_textures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	// Load textures from material of type given
	std::vector<Texture> textures;

#ifdef DEV
	std::cout << mat->GetTextureCount(type) << std::endl;
#endif

	for (int i = 0; i < mat->GetTextureCount(type); i++) {
		// Get texture path and load
		Texture texture;
		aiString texturePath;
		mat->GetTexture(type, i, &texturePath);

#ifdef DEV
		std::cout << m_directory << std::endl;
		std::cout << texturePath.C_Str() << std::endl;
#endif

		// Check if texture has already been loaded
		bool preLoaded = false;
		for (unsigned int j = 0; j < g_loadedTextures.size(); j++) {
			if (g_loadedTextures[j].path == texturePath.C_Str()) { preLoaded = true; textures.push_back(g_loadedTextures[j]); break; }
		}

		if (!preLoaded) {
			// Create texture
			if (typeName == "diffuse") {
				texture.id = create_texture(m_directory + "/" + texturePath.C_Str());
			}
			else {
				texture.id = create_texture(m_directory + "/" + texturePath.C_Str());
			}

			// Set texture type
			texture.type = typeName;

			// Store texture path
			texture.path = texturePath.C_Str();

			// Push back
			g_loadedTextures.push_back(texture);
			textures.push_back(texture);
		}
	}

	return textures;
}

/**
 * @brief Call draw function of all submeshes.
 * 
 * @param shaderProgram 
 * @param camera 
 */
void Model::draw(unsigned int shaderProgram, Camera camera, std::vector<Light> sceneLights) {
	// Draw every submesh
	for (int i = 0; i < m_meshes.size(); i++) {
		m_meshes[i].draw(shaderProgram, camera, m_worldPos, sceneLights);
	}
}

/**
 * @brief Set model location.
 * 
 * @param loc 
 */
void Model::set_loc(glm::vec3 loc) {
	// Set models world pos
	m_loc = loc;
	m_worldPos = glm::translate(m_worldPos, loc);
}

/**
 * @brief Set model scale.
 * 
 * @param scale 
 */
void Model::set_scale(glm::vec3 scale) {
	// Set modles scale
	m_scale = scale;
	m_worldPos = glm::scale(m_worldPos, scale);
}
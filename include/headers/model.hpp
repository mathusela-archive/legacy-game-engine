#pragma once

#include <vector>

#include "headers/structs.hpp"
#include "headers/camera.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/**
 * @brief Individual mesh with geometry, stores its own textures, verticies and indicies.
 * 
 */
class Mesh {
private:
	unsigned int m_VAO, m_VBO, m_EBO;
	void initMesh();

public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	void draw(unsigned int shaderProgram, Camera camera, glm::mat4 worldPos, std::vector<Light> sceneLights);

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

};

/**
 * @brief Individual model, renderable object storing a number of submeshes; Loads mesh geometry from file and generates submeshes to store this.
 * 
 */
class Model {
private:
	glm::vec3 m_loc;
	glm::mat4 m_worldPos;

	glm::vec3 m_scale;

	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void load_model(std::string path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> load_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	Model(std::string path, glm::vec3 spawnLocation);

	void draw(unsigned int shaderProgram, Camera camera, std::vector<Light> sceneLights);

	void set_loc(glm::vec3 loc);
	void set_scale(glm::vec3 scale);

};
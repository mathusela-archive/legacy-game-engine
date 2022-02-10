#include "headers/terrain.hpp"

#include <iostream>

btVector3 glmVec_to_bulletVec(glm::vec3 vecIn) {
	return btVector3(vecIn.x, vecIn.y, vecIn.z);
}

Terrain::Terrain(std::string renderPath, std::string collisionPath, glm::vec3 spawnLocation, btDynamicsWorld* dynamicsWorld) : Object(renderPath, spawnLocation, 0.0, dynamicsWorld) {
	// Import file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(collisionPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// Print any errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	
	std::vector<btConvexTriangleMeshShape*> collisionShapeArray;
	process_node_physics(scene->mRootNode, scene, collisionShapeArray);

	auto collisionShape = new btCompoundShape;
	auto defaultTransform = btTransform(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.0, 0.0));
	for (auto shape : collisionShapeArray) collisionShape->addChildShape(defaultTransform, shape);
	
	// Concave shapes should be modeled as different objects
	m_rigidbody = create_rigidbody(collisionShape, btVector3(spawnLocation.x, spawnLocation.y, spawnLocation.z), 0.0);
	dynamicsWorld->addRigidBody(m_rigidbody);
};

void Terrain::process_node_physics(aiNode* node, const aiScene* scene, std::vector<btConvexTriangleMeshShape*>& collisionShapesReturn) {
	// Process meshes
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto meshOut = process_mesh(mesh, scene);
		auto triangleMesh = new btTriangleMesh();
		
		std::vector<glm::vec3> tris;
		for (int i=0; i<meshOut.m_indices.size(); i+=3) {
			tris.push_back(glm::vec3(meshOut.m_indices[i], meshOut.m_indices[i+1], meshOut.m_indices[i+2]));
		}

		for (glm::vec3 tri : tris) {
			triangleMesh->addTriangle(glmVec_to_bulletVec(meshOut.m_vertices[tri.x].pos), glmVec_to_bulletVec(meshOut.m_vertices[tri.y].pos), glmVec_to_bulletVec(meshOut.m_vertices[tri.z].pos));
		}
		auto meshInterface = new btTriangleIndexVertexArray();
		auto triangleMeshShape = new btConvexTriangleMeshShape(triangleMesh);
		collisionShapesReturn.push_back(triangleMeshShape);
	}

	// Do the same for children
	for (int i = 0; i < node->mNumChildren; i++) {
		process_node_physics(node->mChildren[i], scene, collisionShapesReturn);
	}
}
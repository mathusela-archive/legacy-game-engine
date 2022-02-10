#pragma once

#include "headers/object.hpp"

class Terrain : public Object {
private:
	void process_node_physics(aiNode* node, const aiScene* scene, std::vector<btConvexTriangleMeshShape*>& collisionShapesReturn);

public:
	Terrain(std::string renderPath, std::string collisionPath, glm::vec3 spawnLocation, btDynamicsWorld* dynamicsWorld);

};
#pragma once

#include "headers/model.hpp"
#include "headers/physics-functions.hpp"

class Object : public Model {
protected:
	btDynamicsWorld* m_dynamicsWorld;

public:
	btRigidBody* m_rigidbody;
	void update_render_to_physics();
	Object(std::string path, glm::vec3 spawnLocation, btCollisionShape* collisionShape, float mass, btDynamicsWorld* dynamicsWorld);
	Object(std::string path, glm::vec3 spawnLocation, float mass, btDynamicsWorld* dynamicsWorld);
};
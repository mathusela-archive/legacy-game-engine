#include "headers/object.hpp"

Object::Object(std::string path, glm::vec3 spawnLocation, btCollisionShape* collisionShape, float mass, btDynamicsWorld* dynamicsWorld) : Model(path, spawnLocation) {
	m_rigidbody = create_rigidbody(collisionShape, btVector3(spawnLocation.x, spawnLocation.y, spawnLocation.z), mass);
	m_dynamicsWorld = dynamicsWorld;
	m_dynamicsWorld -> addRigidBody(m_rigidbody);
}

void Object::update_render_to_physics() {
	btTransform trans;
	m_rigidbody -> getMotionState() -> getWorldTransform(trans);
	set_loc(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	set_rot(trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
}
/**
 * @file object.cpp
 * @author Matthew Richardson
 * @brief Define object class; objects with both rendering and physics properties (rigidbody).
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/object.hpp"

/**
 * @brief Construct a new Object instance allowing a separate render model and physics model to be used.
 * 
 * @param path 
 * @param spawnLocation 
 * @param collisionShape 
 * @param mass 
 * @param dynamicsWorld 
 */
Object::Object(std::string path, glm::vec3 spawnLocation, btCollisionShape* collisionShape, float mass, btDynamicsWorld* dynamicsWorld) : Model(path, spawnLocation) {
	m_rigidbody = create_rigidbody(collisionShape, btVector3(spawnLocation.x, spawnLocation.y, spawnLocation.z), mass);
	m_dynamicsWorld = dynamicsWorld;
	m_dynamicsWorld -> addRigidBody(m_rigidbody);
}

/**
 * @brief Construct a new Object instance without initializing a rigidbody. This constructor is used by statics which generate collision meshes on construction (see terrain), not recommended for general use.
 * 
 * @param path 
 * @param spawnLocation 
 * @param mass 
 * @param dynamicsWorld 
 */
Object::Object(std::string path, glm::vec3 spawnLocation, float mass, btDynamicsWorld* dynamicsWorld) : Model(path, spawnLocation) {
	m_dynamicsWorld = dynamicsWorld;
}

/**
 * @brief Update the objects render transforms to match its physics properties.
 * 
 */
void Object::update_render_to_physics() {
	btTransform trans;
	m_rigidbody -> getMotionState() -> getWorldTransform(trans);
	set_loc(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	set_rot(trans.getRotation().getAngle(), glm::vec3(trans.getRotation().getAxis().getX(), trans.getRotation().getAxis().getY(), trans.getRotation().getAxis().getZ()));
}
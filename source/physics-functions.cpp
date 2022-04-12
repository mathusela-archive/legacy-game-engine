/**
 * @file physics-functions.cpp
 * @author Matthew Richardson
 * @brief File containing standard physics helper functions.
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/physics-functions.hpp"

/**
 * @brief Initialize and create dynamics world.
 * 
 * @return btDiscreteDynamicsWorld* 
 */
btDiscreteDynamicsWorld* create_physics_world() {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	return dynamicsWorld;

	//TODO: Delete all of these variables at the end of the program somehow.
}

/**
 * @brief Initialize and create a rigidbody instance.
 * 
 * @param collisionShapeIn 
 * @param initialLocation 
 * @param mass 
 * @return btRigidBody* 
 */
btRigidBody* create_rigidbody(btCollisionShape* collisionShapeIn, btVector3 initialLocation, btScalar mass) {
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(initialLocation);

	btVector3 localInertia(0, 0, 0);
	collisionShapeIn -> calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShapeIn, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	return body;
}
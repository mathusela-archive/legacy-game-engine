#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

btDiscreteDynamicsWorld* create_physics_world();

btRigidBody* create_rigidbody(btCollisionShape* collisionShapeIn, btVector3 initialLocation, btScalar mass);
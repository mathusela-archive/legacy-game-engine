#include "headers/scripts/character-controller.hpp"
#include <iostream>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

bool firstRun = true;

Camera* varCamera;

float verticalVelocity = 0.0;
float cameraDisplacement = 0.0;

btGhostObject* playerGhost;

void character_controller(Camera* camera, GLFWwindow* window, Object& player, float deltaTime, btCollisionShape* shape, float height, btDynamicsWorld* dynamicsWorld) {
	if (firstRun) {
			player.m_rigidbody->setAngularFactor(0); 
			firstRun = false;
			player.m_rigidbody -> setCollisionFlags( player.m_rigidbody -> getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			player.m_rigidbody -> setActivationState(DISABLE_DEACTIVATION);
			varCamera = camera;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			auto ghostPairCallback = new btGhostPairCallback(); 
			dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCallback);

			playerGhost = new btGhostObject();
			playerGhost->setCollisionShape(shape);
			playerGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
			dynamicsWorld->addCollisionObject(playerGhost);
			playerGhost->setWorldTransform(player.m_rigidbody->getWorldTransform());
		}

	// btTransform trans;
	auto trans = player.m_rigidbody -> getCenterOfMassTransform();

	// Test if on ground
	bool onGround = false;
	btVector3 from(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	btVector3 to(trans.getOrigin().getX(), trans.getOrigin().getY() - height, trans.getOrigin().getZ());
	btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
	dynamicsWorld -> rayTest(from, to, rayCallback);
	if (rayCallback.hasHit()) onGround = true;

	// Set speed
	float cameraShake = 5.0;
	float baseSpeed = 1.6;
    float sprintSpeed = 3.0;
    float slowSpeed = 0.5;
    float objectSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {objectSpeed = sprintSpeed; cameraShake = 7.0;}
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {objectSpeed = slowSpeed; cameraShake = 3.0;}
    else objectSpeed = baseSpeed;

	auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Inputs
	auto pos = player.m_rigidbody -> getCenterOfMassPosition();

	btVector3 dir = btVector3(0.0, 0.0, 0.0);
	float verticalDir = 0.0;
	float verticalAccelleration = 0.0;
	if(!onGround) verticalAccelleration = -10.0;
	if(onGround) verticalVelocity = 0.0;

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			dir += btVector3(camera->m_front.x, 0.0, camera->m_front.z).normalize();
		}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			dir -= btVector3(camera->m_front.x, 0.0, camera->m_front.z).normalize();
		}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			auto rightVec = glm::cross(camera->m_front, cameraUp);
			dir -= btVector3(rightVec.x, rightVec.y, rightVec.z);
		}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			auto rightVec = glm::cross(camera->m_front, cameraUp);
			dir += btVector3(rightVec.x, rightVec.y, rightVec.z);
		}
		if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			verticalVelocity = -10;
		}
	if (onGround && (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) cameraDisplacement += cameraShake * deltaTime;
	if(onGround && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) verticalVelocity += 7;

	verticalDir = verticalVelocity * deltaTime;
	verticalVelocity += verticalAccelleration * deltaTime;
	
	// Set dir
	if (dir.length() != 0) dir = dir.normalize();
	dir = dir * objectSpeed * deltaTime;

	// Static collisions
	playerGhost->setWorldTransform(player.m_rigidbody->getWorldTransform());
	auto objectArray = playerGhost->getOverlappingPairs();
	for (int j=0; j<objectArray.size(); j++) if (objectArray[j]->isStaticObject()) {
		for (int i=0; i<dynamicsWorld->getDispatcher()->getNumManifolds(); i++) {
			objectArray[j]->activate(true);
			auto contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			if ((contactManifold->getBody0() == player.m_rigidbody || contactManifold->getBody1() == player.m_rigidbody) && (contactManifold->getBody0() == objectArray[j] || contactManifold->getBody1() == objectArray[j])) {
				contactManifold->refreshContactPoints(objectArray[j]->getWorldTransform(), player.m_rigidbody->getWorldTransform());
				float smallerIndex = 0, smallerValue = 10000;
				for (int k=0; k<contactManifold->getNumContacts(); k++) {
					auto pt = contactManifold->getContactPoint(k); 
					if ((pt.getPositionWorldOnB() - pt.getPositionWorldOnA()).length() < smallerValue) {
						smallerValue = (pt.getPositionWorldOnB() - pt.getPositionWorldOnA()).length();
						smallerIndex = k;
					}
				}
				if (contactManifold->getNumContacts() > 0) {
					auto pt = contactManifold->getContactPoint(smallerIndex);
					auto projection = (dir.dot(pt.m_normalWorldOnB))/(pow(pt.m_normalWorldOnB.length(), 2))*pt.m_normalWorldOnB; // Project dir to normal
					auto prodDir = pt.m_normalWorldOnB*dir;
					projection.setX(prodDir.getX() < 0.0 ? 0 : projection.getX()); 
					projection.setY(prodDir.getY() < 0.0 ? 0 : projection.getY()); 
					projection.setZ(prodDir.getZ() < 0.0 ? 0 : projection.getZ()); 
					dir -= projection;

					// FIXME: May not work with collisions from below (i.e. jumping into ceiling)
				}
			}
		}
	};

	{
	// Test if has hit ceiling
	btVector3 from(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
	btVector3 to(trans.getOrigin().getX(), trans.getOrigin().getY() + height, trans.getOrigin().getZ());
	btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
	dynamicsWorld -> rayTest(from, to, rayCallback);
	// This statement relies on the compiler optimising to exit early if first statement is false, will crash if not for this (mileage may vary with other compilers)
	if (rayCallback.hasHit() && rayCallback.m_collisionObject->isStaticObject()) verticalVelocity = -1.5;
	}

	// Transform
	trans.setOrigin(btVector3(pos.getX() + dir.getX(), pos.getY() + verticalDir, pos.getZ() + dir.getZ()));
	player.m_rigidbody -> getMotionState() -> setWorldTransform(trans);

	camera -> set_loc(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY() + height/2.0 +  0.07*(0.5*pow(sin(cameraDisplacement - 1), 2) + 0.25 * pow(cos(cameraDisplacement), 3)), trans.getOrigin().getZ()));
	
	glfwSetCursorPosCallback(window, mouse_callback);

    camera->m_pos = glm::lookAt(camera->m_loc,
        camera->m_loc + camera->m_front,
        cameraUp);
	
	// TODO: Add check to stop going through statics: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=3026
	// TODO: Add force to push you out of objects if you're inside them (right now just stops you)
	// FIXME: Fix stutters
	// FIXME: Not working at all framerates
}


float lastx = 1100 / 2;
float lasty = 700 / 2;

float yaw = 90.0f;
float pitch = 0.0f;

const float sensitivity = 0.1f;

/**
 * @brief Handle mouse input for camera control.
 * 
 * @param window 
 * @param xpos 
 * @param ypos 
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;
    lastx = xpos;
    lasty = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    varCamera->m_front = glm::normalize(direction);
}
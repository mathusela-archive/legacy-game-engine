#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/camera.hpp"
#include "headers/object.hpp"

void character_controller(Camera* camera, GLFWwindow* window, Object& player, float deltaTime, btCollisionShape* shape, float height, btDynamicsWorld* dynamicsWorld);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "headers/camera.hpp"

void camera_controller_init(GLFWwindow* window);

void camera_controller(Camera* camera, GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
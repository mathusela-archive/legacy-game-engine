/**
 * @file camera-controller.cpp
 * @author Matthew Richardson
 * @brief File storing scripts related to camera control.
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/scripts/camera-controller.hpp"

/**
 * @brief Initiate initial screen settings for GLFW window. 
 * 
 * @param window 
 */
void camera_controller_init(GLFWwindow* window) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

float deltaTime;
float lastFrame;

float yaw = 90.0f;
float pitch = 0.0f;

Camera* varCamera;

bool captureCursorToggle = true;
int prevKey = 0;

/**
 * @brief Handle keyboard for camera control as well as calling mouse handler.
 * 
 * @param camera 
 * @param window 
 */
void camera_controller(Camera* camera, GLFWwindow* window) {
    varCamera = camera;

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float baseSpeed = 4.0;
    float sprintSpeed = 8.0;
    float slowSpeed = 1.0;
    float cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraSpeed = sprintSpeed * deltaTime;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraSpeed = slowSpeed * deltaTime;
    else cameraSpeed = baseSpeed * deltaTime;

    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->m_loc += cameraSpeed * glm::normalize(glm::vec3(camera->m_front.x, 0.0, camera->m_front.z));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->m_loc -= cameraSpeed * glm::normalize(glm::vec3(camera->m_front.x, 0.0, camera->m_front.z));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->m_loc -= glm::normalize(glm::cross(camera->m_front, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->m_loc += glm::normalize(glm::cross(camera->m_front, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera->m_loc += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->m_loc -= cameraSpeed * cameraUp;

    void mouse_callback(GLFWwindow * window, double xpos, double ypos);

#ifndef FULLSCREEN
    if (!prevKey) {
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            if (captureCursorToggle) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                captureCursorToggle = false;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                captureCursorToggle = true;
            }
        }
    }
#else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
    prevKey = glfwGetKey(window, GLFW_KEY_TAB);

    glfwSetCursorPosCallback(window, mouse_callback);

    camera->m_pos = glm::lookAt(camera->m_loc,
        camera->m_loc + camera->m_front,
        cameraUp);

}

float lastx = 1100 / 2;
float lasty = 700 / 2;

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
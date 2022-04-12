/**
 * @file camera.cpp
 * @author Matthew Richardson
 * @brief Define camera class used for rendering and generating view matrices.
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/camera.hpp"

/**
 * @brief Set camera location to a specific position and update translation matrix.
 * 
 * @param setLoc 
 */
void Camera::set_loc(glm::vec3 setLoc) {
	m_loc = setLoc;
	m_pos = glm::mat4(1.0f);
	m_pos = glm::translate(m_pos, m_loc);
}

/**
 * @brief Move cameras location relative to its current position and update translation matrix.
 * 
 * @param moveLoc 
 */
void Camera::move_loc(glm::vec3 moveLoc) {
	m_loc = m_loc + moveLoc;
	set_loc(m_loc);
}

/**
 * @brief Construct a new Camera object.
 * 
 * @param fov 
 * @param width 
 * @param height 
 * @param nearClipping 
 * @param farClipping 
 * @param loc 
 * @param front 
 */
Camera::Camera(float fov, int width, int height, float nearClipping, float farClipping, glm::vec3 loc, glm::vec3 front) {
	m_projection = glm::perspective(glm::radians(fov), (float)width / height, nearClipping, farClipping);
	set_loc(loc);
	m_front = front;
}
/**
 * @file camera.cpp
 * @author Matthew Richardson
 * @brief Define camera class used for rendering and generating view matrices
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/camera.hpp"

void Camera::set_loc(glm::vec3 setLoc) {
	m_loc = setLoc;
	m_pos = glm::mat4(1.0f);
	m_pos = glm::translate(m_pos, m_loc);
}

void Camera::move_loc(glm::vec3 moveLoc) {
	m_loc = m_loc + moveLoc;
	set_loc(m_loc);
}

Camera::Camera(float fov, int width, int height, float nearClipping, float farClipping, glm::vec3 loc, glm::vec3 front) {
	m_projection = glm::perspective(glm::radians(fov), (float)width / height, nearClipping, farClipping);
	set_loc(loc);
	m_front = front;
}
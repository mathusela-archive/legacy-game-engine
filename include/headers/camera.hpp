#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

/**
 * @brief Camera class, used for rendering and generating view matrices.
 * 
 */
class Camera {
public:
	glm::vec3 m_loc;
	glm::mat4 m_pos;
	glm::mat4 m_projection;
	glm::vec3 m_front;

	void set_loc(glm::vec3 setLoc);

	void move_loc(glm::vec3 moveLoc);

	Camera(float fov, int width, int height, float nearClipping, float farClipping, glm::vec3 loc, glm::vec3 front);
};
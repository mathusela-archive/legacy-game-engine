/**
 * @file screen-quad.cpp
 * @author Matthew Richardson
 * @brief Define screen quad class.
 * @version 0.1
 * @date 2022-02-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "headers/screen-quad.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/standard-functions.hpp"

/**
 * @brief Draw the screen quad.
 * 
 * @param shaderProgram 
 * @param texture 
 */
void ScreenQuad::draw(unsigned int shaderProgram, unsigned int texture) {
	glBindVertexArray(m_VAO);
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "render"), 0);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

/**
 * @brief Construct a new Screen Quad object.
 * 
 */
ScreenQuad::ScreenQuad() {
	m_VAO = create_vao(m_mesh);
}

/**
 * @brief Initialize the screen quad's VAO (geometry).
 * 
 * @param verts 
 * @return VAO 
 */
unsigned int ScreenQuad::create_vao(std::vector<float> verts) {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	return VAO;
}
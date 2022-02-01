/**
 * @file screen-quad.cpp
 * @author Matthew Richardson
 * @brief 
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

void ScreenQuad::draw(unsigned int texture) {
	glBindVertexArray(m_VAO);
	glUseProgram(m_shaderProgram);
	glUniform1i(glGetUniformLocation(m_shaderProgram, "render"), 0);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

ScreenQuad::ScreenQuad(unsigned int shaderProgram) {
	m_shaderProgram = shaderProgram;
	m_VAO = create_vao(m_mesh);
}

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
#pragma once

#include <vector>

/**
 * @brief Quad which takes up the whole screen; used for post processing.
 * 
 */
class ScreenQuad {
	public:
		std::vector<float> m_mesh = {
				-1.0f, -1.0f, 0.0f,		0.0, 0.0,
				-1.0f, 1.0f, 0.0f,		0.0, 1.0,
				1.0f, 1.0f, 0.0f,		1.0, 1.0,

				-1.0f, -1.0f, 0.0f,		0.0, 0.0,
				1.0f, -1.0f, 0.0f,		1.0, 0.0,
				1.0f, 1.0f, 0.0f,		1.0, 1.0,
			};

		unsigned int m_VAO;
		unsigned int m_shaderProgram;

		unsigned int create_vao(std::vector<float> verts);

		void draw(unsigned int texture);

		ScreenQuad(unsigned int shaderProgram);
};
/**
 * @file main.cpp
 * @author Matthew Richardson
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define DEV

#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/standard-functions.hpp"
#include "headers/camera.hpp"
#include "headers/model.hpp"
#include "headers/scripts/camera-controller.hpp"

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 800;
char TITLE[] = "Dungeon Crawler";

const std::string ROOT_DIR = "../../";

int main() {
	auto window = create_window(WIDTH, HEIGHT, TITLE);

	auto shaderProgram = create_shader(ROOT_DIR + "resources/shaders/solid/vertex-shader.vert", ROOT_DIR + "resources/shaders/solid/fragment-shader.frag");
	Camera camera(90.0f, WIDTH, HEIGHT, 0.02f, 100.0f, glm::vec3 {0.0, 0.0, 0.0}, glm::vec3 {0.0, 0.0, 1.0}); 
	Model test(ROOT_DIR + "resources/models/spec-cube/specCube.obj", glm::vec3 {0.0, 0.0, -10.0});

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		camera_controller(&camera, window);

		test.draw(shaderProgram, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
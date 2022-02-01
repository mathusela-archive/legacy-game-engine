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
#include "headers/screen-quad.hpp"

const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;
char TITLE[] = "Dungeon Crawler";

const std::string ROOT_DIR = "../../";

int main() {
	auto window = create_window(WIDTH, HEIGHT, TITLE, 4);
	unsigned int hdrRenderTexture; 
	auto hdrFramebuffer = create_framebuffer(hdrRenderTexture, WIDTH, HEIGHT);

	auto shaderProgram = create_shader(ROOT_DIR + "resources/shaders/solid/vertex-shader.vert", ROOT_DIR + "resources/shaders/solid/fragment-shader.frag");
	Camera camera(90.0f, WIDTH, HEIGHT, 0.02f, 100.0f, glm::vec3 {0.0, 1.0, 0.0}, glm::vec3 {0.0, 0.0, 1.0}); 
	Model cube(ROOT_DIR + "resources/models/spec-cube/specCube.obj", glm::vec3 {0.0, 1.0, 10.0});
	Model plane(ROOT_DIR + "resources/models/plane/Ground.obj", glm::vec3(0.0, 0.0, 0.0)); plane.set_scale(glm::vec3(10.0));
	Model hut(ROOT_DIR + "resources/models/hut/HutHigh.obj", glm::vec3(0.0, 0.0, 0.0)); hut.set_scale(glm::vec3(0.4));
	std::vector<Light> sceneLights = {
		Light {glm::vec3{5.0, 5.0, 8.0}, glm::vec3{1.0, 1.0, 1.0}, 1.0, POINT},
		Light {glm::vec3{-5.0, 5.0, 8.0}, glm::vec3{0.0, 1.0, 1.0}, 1.0, POINT},
		Light {glm::vec3{-10.0, 1.0, -1.0}, glm::vec3{0.0, 1.0, 0.0}, 1.0, POINT},
		Light {glm::vec3 {-100.0, 100.0, 80.0}, glm::vec3{0.71, 0.70, 0.50}, 8.0, DIRECTIONAL}
	};

	auto screenQuadShader = create_shader(ROOT_DIR + "resources/shaders/screen-quad/vertex-shader.vert", ROOT_DIR + "resources/shaders/screen-quad/fragment-shader.frag");
	ScreenQuad screenQuad(screenQuadShader);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(0);

	float exposure = 1.0;
	float lastFrame = 0.0;
	float deltaTime;
	
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFramebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera_controller(&camera, window);

		cube.draw(shaderProgram, camera, sceneLights);
		plane.draw(shaderProgram, camera, sceneLights);
		hut.draw(shaderProgram, camera, sceneLights);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		calculate_exposure(hdrFramebuffer, exposure, deltaTime, 0.5, 0.3, 5.0, 3.0);
		glUseProgram(screenQuadShader);
		glUniform1f(glGetUniformLocation(screenQuadShader, "exposure"), exposure);
		screenQuad.draw(hdrRenderTexture);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
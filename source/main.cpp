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
	unsigned int renderTexture;
	auto renderFramebuffer = create_framebuffer(std::vector<unsigned int*> {&renderTexture}, WIDTH, HEIGHT);
	unsigned int hdrRenderTexture; unsigned int brightPixelsTexture;
	auto hdrFramebuffer = create_framebuffer(std::vector<unsigned int*> {&hdrRenderTexture, &brightPixelsTexture}, WIDTH, HEIGHT);
	unsigned int blurTextures[2];
	unsigned int blurFramebuffers[2] = {
		create_framebuffer( std::vector<unsigned int*> {&blurTextures[0]}, WIDTH, HEIGHT),
		create_framebuffer(std::vector<unsigned int*> {&blurTextures[1]}, WIDTH, HEIGHT)
	};

	auto shaderProgram = create_shader(ROOT_DIR + "resources/shaders/solid/vertex-shader.vert", ROOT_DIR + "resources/shaders/solid/fragment-shader.frag");
	Camera camera(90.0f, WIDTH, HEIGHT, 0.02f, 100.0f, glm::vec3 {0.0, 1.0, 0.0}, glm::vec3 {0.0, 0.0, 1.0}); 
	Model cube(ROOT_DIR + "resources/models/spec-cube/specCube.obj", glm::vec3 {0.0, 1.0, 10.0});
	Model plane(ROOT_DIR + "resources/models/plane/Ground.obj", glm::vec3(0.0, 0.0, 0.0)); plane.set_scale(glm::vec3(10.0));
	Model hut(ROOT_DIR + "resources/models/hut/HutHigh.obj", glm::vec3(0.0, 0.0, 0.0)); hut.set_scale(glm::vec3(0.4));
	std::vector<Light> sceneLights = {
		Light {glm::vec3{5.0, 5.0, 8.0}, glm::vec3{1.0, 1.0, 1.0}, 1.0, POINT},
		Light {glm::vec3{-5.0, 5.0, 8.0}, glm::vec3{0.0, 1.0, 1.0}, 1.0, POINT},
		Light {glm::vec3{-10.0, 1.0, -1.0}, glm::vec3{0.0, 1.0, 0.0}, 1.0, POINT},
		Light {glm::vec3 {-100.0, 100.0, 80.0}, glm::vec3{0.71, 0.50, 0.50}, 8.0, DIRECTIONAL}
	};

	auto hdrShader = create_shader(ROOT_DIR + "resources/shaders/hdr/vertex-shader.vert", ROOT_DIR + "resources/shaders/hdr/fragment-shader.frag");
	auto blurShader = create_shader(ROOT_DIR + "resources/shaders/blur/vertex-shader.vert", ROOT_DIR + "resources/shaders/blur/fragment-shader.frag");
	auto standardPostprocessingShader = create_shader(ROOT_DIR + "resources/shaders/standard-postprocessing/vertex-shader.vert", ROOT_DIR + "resources/shaders/standard-postprocessing/fragment-shader.frag");
	auto bloomShader = create_shader(ROOT_DIR + "resources/shaders/bloom/vertex-shader.vert", ROOT_DIR + "resources/shaders/bloom/fragment-shader.frag");
	ScreenQuad screenQuad;

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(0);

	float exposure = 0.3;
	float lastFrame = 0.0;
	float deltaTime;
	
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera_controller(&camera, window);

		cube.draw(shaderProgram, camera, sceneLights);
		plane.draw(shaderProgram, camera, sceneLights);
		hut.draw(shaderProgram, camera, sceneLights);

		auto bluredTexture = run_gaussian_blur(blurShader, brightPixelsTexture, blurFramebuffers, blurTextures, 10);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFramebuffer);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		calculate_exposure(renderTexture, exposure, deltaTime, 0.5, 0.3, 5.0, 3.0);
		glUseProgram(hdrShader);
		glUniform1f(glGetUniformLocation(hdrShader, "exposure"), exposure);
		screenQuad.draw(hdrShader, renderTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(bloomShader);
		glUniform1f(glGetUniformLocation(bloomShader, "exposure"), exposure);
		glUniform1i(glGetUniformLocation(bloomShader, "blur"), 1);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, bluredTexture);
		screenQuad.draw(bloomShader, hdrRenderTexture);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
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
#include "headers/physics-functions.hpp"
#include "headers/object.hpp"
#include "headers/scripts/character-controller.hpp"
#include "headers/terrain.hpp"

const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;
char TITLE[] = "Game Engine";

const std::string ROOT_DIR = "../../";

// Functions
void deferred_rendering_uniforms(unsigned int shaderProgram, Camera camera, std::vector<Light> sceneLights,  unsigned int colorBuffer, unsigned int positionBuffer, unsigned int normalSpecularBuffer) {
	glUseProgram(shaderProgram);
	// Textures
	glUniform1i(glGetUniformLocation(shaderProgram, "colorMap"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "positionMap"), 2);
	glUniform1i(glGetUniformLocation(shaderProgram, "normal_specularMap"), 3);
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, positionBuffer);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, normalSpecularBuffer);
	// Camera
	glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(camera.m_loc));
	// Lighting
	for (int i=0; i<sceneLights.size(); i++) {
		std::string posName = "sceneLights[" + std::to_string(i) + "].position";
		std::string colorName = "sceneLights[" + std::to_string(i) + "].color";
		std::string powerName = "sceneLights[" + std::to_string(i) + "].power";
		std::string typeName = "sceneLights[" + std::to_string(i) + "].type";

		glUniform3fv(glGetUniformLocation(shaderProgram, posName.c_str()), 1, glm::value_ptr(sceneLights[i].location));
		glUniform3fv(glGetUniformLocation(shaderProgram, colorName.c_str()), 1, glm::value_ptr(sceneLights[i].color));
		glUniform1f(glGetUniformLocation(shaderProgram, powerName.c_str()), sceneLights[i].power);
		glUniform1i(glGetUniformLocation(shaderProgram, typeName.c_str()), sceneLights[i].type);
	}
	glUniform1i(glGetUniformLocation(shaderProgram, "lightsCount"), sceneLights.size());
}

int main() {
	// Rendering
	auto window = create_window(WIDTH, HEIGHT, TITLE, 4);

	// Framebuffers
	unsigned int colorBuffer, positionBuffer, normalSpecularBuffer;
	auto gBuffer = create_framebuffer(std::vector<unsigned int*> {&colorBuffer, &positionBuffer, &normalSpecularBuffer}, WIDTH, HEIGHT);

	unsigned int renderTexture;
	auto renderFramebuffer = create_framebuffer(std::vector<unsigned int*> {&renderTexture}, WIDTH, HEIGHT);
	unsigned int hdrRenderTexture; unsigned int brightPixelsTexture;
	auto hdrFramebuffer = create_framebuffer(std::vector<unsigned int*> {&hdrRenderTexture, &brightPixelsTexture}, WIDTH, HEIGHT);
	unsigned int blurTextures[2];
	unsigned int blurFramebuffers[2] = {
		create_framebuffer(std::vector<unsigned int*> {&blurTextures[0]}, WIDTH, HEIGHT),
		create_framebuffer(std::vector<unsigned int*> {&blurTextures[1]}, WIDTH, HEIGHT)
	};

	
	// Lights and cameras
	Camera camera(90.0f, WIDTH, HEIGHT, 0.02f, 100.0f, glm::vec3 {0.0, 1.0, 0.0}, glm::vec3 {0.0, 0.0, 1.0}); 
	std::vector<Light> sceneLights = {
		Light {glm::vec3{5.0, 5.0, 8.0}, glm::vec3{1.0, 1.0, 1.0}, 1.0, POINT},
		Light {glm::vec3{-5.0, 5.0, 8.0}, glm::vec3{0.0, 1.0, 1.0}, 1.0, POINT},
		Light {glm::vec3{-10.0, 1.0, -1.0}, glm::vec3{0.0, 1.0, 0.0}, 1.0, POINT},
		Light {glm::vec3 {-100.0, 100.0, 80.0}, glm::vec3{0.71, 0.50, 0.50}, 8.0, DIRECTIONAL}
	};

	// Shaders
	auto deferredGeometryShader = create_shader(ROOT_DIR + "resources/shaders/deferred/geometry/vertex-shader.vert", ROOT_DIR + "resources/shaders/deferred/geometry/fragment-shader.frag");
	auto deferredShaderProgram = create_shader(ROOT_DIR + "resources/shaders/deferred/solid/vertex-shader.vert", ROOT_DIR + "resources/shaders/deferred/solid/fragment-shader.frag");

	// auto shaderProgram = create_shader(ROOT_DIR + "resources/shaders/forward/solid/vertex-shader.vert", ROOT_DIR + "resources/shaders/forward/solid/fragment-shader.frag");
	auto hdrShader = create_shader(ROOT_DIR + "resources/shaders/forward/hdr/vertex-shader.vert", ROOT_DIR + "resources/shaders/forward/hdr/fragment-shader.frag");
	auto blurShader = create_shader(ROOT_DIR + "resources/shaders/forward/blur/vertex-shader.vert", ROOT_DIR + "resources/shaders/forward/blur/fragment-shader.frag");
	auto standardPostprocessingShader = create_shader(ROOT_DIR + "resources/shaders/forward/standard-postprocessing/vertex-shader.vert", ROOT_DIR + "resources/shaders/forward/standard-postprocessing/fragment-shader.frag");
	auto bloomShader = create_shader(ROOT_DIR + "resources/shaders/forward/bloom/vertex-shader.vert", ROOT_DIR + "resources/shaders/forward/bloom/fragment-shader.frag");
	ScreenQuad screenQuad;

	// Physics
	auto dynamicsWorld = create_physics_world();
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	auto groundCollisionShape = new btBoxShape(btVector3(50, 4, 50));
	auto boxCollisionShape = new btBoxShape(btVector3(1.0, 1.0, 1.0));
	auto playerCollisionShape = new btCapsuleShape(0.3, 1.0);
	collisionShapes.push_back(groundCollisionShape);
	collisionShapes.push_back(boxCollisionShape);
	collisionShapes.push_back(playerCollisionShape);

	// Objects
	Model hut(ROOT_DIR + "resources/models/hut/HutHigh.obj", glm::vec3(0.0, 0.0, 0.0)); hut.set_scale(glm::vec3(0.4));
	Object cube(ROOT_DIR + "resources/models/spec-cube/specCube.obj", glm::vec3 {0.0, 10.0, 0.0}, boxCollisionShape, 1.0, dynamicsWorld);
	Terrain room(ROOT_DIR + "resources/models/rooms/test-room/testRoom.obj", ROOT_DIR + "resources/models/rooms/test-room/testRoom.obj", glm::vec3 {0.0, 0.0, 0.0}, dynamicsWorld);
	Object player("", glm::vec3(0.0, 1.0, 0.0), playerCollisionShape, 1.0, dynamicsWorld);

	// Gameloop
	// FIXME: With deferred rendering bloom interacts with clear color in the sky
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	// glfwSwapInterval(0);

	float exposure = 0.3;
	float lastFrame = 0.0;
	float deltaTime;
	
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
		
		// Time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Physics
		dynamicsWorld -> stepSimulation(deltaTime);

		cube.update_render_to_physics();
		player.update_render_to_physics();
		character_controller(&camera, window, player, deltaTime, playerCollisionShape, 1.0, dynamicsWorld);

		// Render
		// Render geometry
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.draw_deferred(deferredGeometryShader, camera);
		room.draw_deferred(deferredGeometryShader, camera);
		hut.draw_deferred(deferredGeometryShader, camera);

		// Render lighting
		glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		deferred_rendering_uniforms(deferredShaderProgram, camera, sceneLights, colorBuffer, positionBuffer, normalSpecularBuffer);
		screenQuad.draw(deferredShaderProgram, renderTexture);

		// HDR tonemapping
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFramebuffer);
		
		calculate_exposure(renderTexture, exposure, deltaTime, 0.5, 0.3, 5.0, 3.0);
		glUseProgram(hdrShader);
		glUniform1f(glGetUniformLocation(hdrShader, "exposure"), exposure);
		screenQuad.draw(hdrShader, renderTexture);

		// Blur
		auto bluredTexture = run_gaussian_blur(blurShader, brightPixelsTexture, blurFramebuffers, blurTextures, 10);
		// Bloom (to default framebuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(bloomShader);
		glUniform1f(glGetUniformLocation(bloomShader, "exposure"), exposure);
		glUniform1i(glGetUniformLocation(bloomShader, "blur"), 1);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, bluredTexture);
		screenQuad.draw(bloomShader, hdrRenderTexture);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	for (int i=dynamicsWorld -> getNumCollisionObjects()-1; i>=0; i--) {
		btCollisionObject* obj = dynamicsWorld -> getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		delete body -> getMotionState();
		dynamicsWorld -> removeCollisionObject(obj);
		delete obj;
	}

	for (int i=0; i<collisionShapes.size(); i++) {
		btCollisionShape* shape = collisionShapes[i];
		collisionShapes[i] = 0;
		delete shape;
	}

	return 0;
}
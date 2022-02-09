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

const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;
char TITLE[] = "Dungeon Crawler";

const std::string ROOT_DIR = "../../";

int main() {
	// Rendering
	auto window = create_window(WIDTH, HEIGHT, TITLE, 4);
	unsigned int renderTexture;
	auto renderFramebuffer = create_framebuffer(std::vector<unsigned int*> {&renderTexture}, WIDTH, HEIGHT);
	unsigned int hdrRenderTexture; unsigned int brightPixelsTexture;
	auto hdrFramebuffer = create_framebuffer(std::vector<unsigned int*> {&hdrRenderTexture, &brightPixelsTexture}, WIDTH, HEIGHT);
	unsigned int blurTextures[2];
	unsigned int blurFramebuffers[2] = {
		create_framebuffer(std::vector<unsigned int*> {&blurTextures[0]}, WIDTH, HEIGHT),
		create_framebuffer(std::vector<unsigned int*> {&blurTextures[1]}, WIDTH, HEIGHT)
	};

	auto shaderProgram = create_shader(ROOT_DIR + "resources/shaders/solid/vertex-shader.vert", ROOT_DIR + "resources/shaders/solid/fragment-shader.frag");
	Camera camera(90.0f, WIDTH, HEIGHT, 0.02f, 100.0f, glm::vec3 {0.0, 1.0, 0.0}, glm::vec3 {0.0, 0.0, 1.0}); 
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

	// Physics
	auto dynamicsWorld = create_physics_world();
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	auto groundCollisionShape = new btBoxShape(btVector3(50, 4, 50));
	auto boxCollisionShape = new btBoxShape(btVector3(1.0, 1.0, 1.0));
	auto playerCollisionShape = new btCapsuleShape(0.3, 1.0);
	collisionShapes.push_back(groundCollisionShape);
	collisionShapes.push_back(boxCollisionShape);
	collisionShapes.push_back(playerCollisionShape);

	Object cube(ROOT_DIR + "resources/models/spec-cube/specCube.obj", glm::vec3 {0.0, 10.0, 0.0}, boxCollisionShape, 1.0, dynamicsWorld);
	Object cube1(ROOT_DIR + "resources/models/spec-cube/specCube.obj", glm::vec3 {0.0, 1.0, 10.0}, boxCollisionShape, 0.0, dynamicsWorld);
	Object plane(ROOT_DIR + "resources/models/plane/Ground.obj", glm::vec3(0.0, -4.0, 0.0), groundCollisionShape, 0.0, dynamicsWorld); plane.set_loc(glm::vec3(0.0, 0.0, 0.0)); plane.set_scale(glm::vec3(10.0));
	Object player("", glm::vec3(0.0, 1.0, 0.0), playerCollisionShape, 1.0, dynamicsWorld);

	// Gameloop
	glClearColor(1.0, 1.0, 1.0, 1.0);
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
		cube1.update_render_to_physics();
		player.update_render_to_physics();
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {cube.m_rigidbody -> activate(true); cube.m_rigidbody -> applyTorque(btVector3(0.0, 0.0, 20.0));}
		character_controller(&camera, window, player, deltaTime, playerCollisionShape, 1.0, dynamicsWorld);

		// Render
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// camera_controller(&camera, window);

		cube.draw(shaderProgram, camera, sceneLights);
		cube1.draw(shaderProgram, camera, sceneLights);
		plane.draw(shaderProgram, camera, sceneLights);
		hut.draw(shaderProgram, camera, sceneLights);

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFramebuffer);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		calculate_exposure(renderTexture, exposure, deltaTime, 0.5, 0.3, 5.0, 3.0);
		glUseProgram(hdrShader);
		glUniform1f(glGetUniformLocation(hdrShader, "exposure"), exposure);
		screenQuad.draw(hdrShader, renderTexture);

		// FIXME: Physics too slow under 60hz.
		// FIXME: Scaling not working with physics (update_render_to_physics).
		// TODO: Look into compound shapes for statics. And/or use triangle mesh colliders: https://docs.panda3d.org/1.10/python/programming/physics/bullet/collision-shapes
		auto bluredTexture = run_gaussian_blur(blurShader, brightPixelsTexture, blurFramebuffers, blurTextures, 10);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(bloomShader);
		glUniform1f(glGetUniformLocation(bloomShader, "exposure"), exposure);
		glUniform1i(glGetUniformLocation(bloomShader, "blur"), 1);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, bluredTexture);
		screenQuad.draw(bloomShader, hdrRenderTexture);

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
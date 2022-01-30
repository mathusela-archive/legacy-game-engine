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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/standard-functions.hpp"

const unsigned int WIDTH = 500;
const unsigned int HEIGHT = 500;
char TITLE[] = "Dungeon Crawler";

int main() {
	auto window = create_window(WIDTH, HEIGHT, TITLE);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
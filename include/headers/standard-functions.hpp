#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* create_window(unsigned int width, unsigned int height, char* title);

unsigned int create_texture(std::string imagePath);

std::string import_shader_source(std::string inputPath);

unsigned int create_shader(std::string vertexShaderPath, std::string fragmentShaderPath);
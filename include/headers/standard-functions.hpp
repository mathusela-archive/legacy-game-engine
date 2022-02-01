#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* create_window(unsigned int width, unsigned int height, char* title, unsigned int MSAA);

unsigned int create_texture(std::string imagePath);

unsigned int create_framebuffer(unsigned int& textureOut, unsigned int width, unsigned int height);

void calculate_exposure(unsigned int texture, float& exposure, float deltaTime, float averageLevel, float lowClip, float highClip, float speed);

std::string import_shader_source(std::string inputPath);

unsigned int create_shader(std::string vertexShaderPath, std::string fragmentShaderPath);
/**
 * @file standard-functions.cpp
 * @author Matthew Richardson
 * @brief Definitions of various standard helper functions which are used throught the codebase.
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <fstream>

#include "headers/standard-functions.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/**
 * @brief Create and configure a GLFW window.
 * 
 * @param width 
 * @param height 
 * @param title 
 * @param MSAA Multisampling level of MSAA.
 * @return Reference to window.
 */
GLFWwindow* create_window(unsigned int width, unsigned int height, char* title, unsigned int MSAA) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, MSAA);

	auto window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, width, height);

	return window;
}

/**
 * @brief Load an external image file and generate an OpenGl texture object from it. 
 * 
 * @param imagePath 
 * @return Reference to texture.
 */
unsigned int create_texture(std::string imagePath) {
    int width, height, colorChannelCount;
    unsigned char* image = stbi_load(imagePath.c_str(), &width, &height, &colorChannelCount, 0);

    GLenum imageFormat = (colorChannelCount == 4) ? GL_RGBA : GL_RGB;

#ifdef DEV
    if (!image) {
        std::cout << "FAILED TO LOAD IMAGE" << std::endl;
    }
#endif

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

    return texture;
}

/**
 * @brief Read in external text file in a format ready to be compiled as a shader.
 * 
 * @param inputPath 
 * @return Raw text from file.
 */
std::string import_shader_source(std::string inputPath) {
    std::ifstream input(inputPath);
    std::string inputText = "";

    for (std::string line; getline(input, line);) {
        inputText = inputText + "\n" + line;
    }

    inputText += "\0";

    input.close();
    input.clear();

    return inputText;
}

#define DEV
/**
 * @brief Generate OpenGL shader program. 
 * 
 * @param vertexShaderPath 
 * @param fragmentShaderPath 
 * @return Reference to shader program.
 */
unsigned int create_shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    std::string vertexShaderSourceString = import_shader_source(vertexShaderPath);
    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

#ifdef DEV
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << vertexShaderSource << "\n" << std::endl;
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    };
#endif

    std::string fragmentShaderSouceString = import_shader_source(fragmentShaderPath);
    const char* fragmentShaderSource = fragmentShaderSouceString.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

#ifdef DEV
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << fragmentShaderSource << "\n" << std::endl;
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    };
#endif

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

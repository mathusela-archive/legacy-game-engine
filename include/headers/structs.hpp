#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

/**
 * @brief Stores various attributes of a vertex.
 * 
 */
struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 tex;
	glm::vec3 tangent;
};

/**
 * @brief Wrapper for textures storing other information with the ID.
 * 
 */
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

enum t_LightType {POINT, DIRECTIONAL};

/**
 * @brief Attribute wrapper for Light objects.
 * 
 */
struct Light {
	glm::vec3 location;
	glm::vec3 color;
	t_LightType type;
};
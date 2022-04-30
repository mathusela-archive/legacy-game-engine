#version 330 core

// Vetex attributes
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoords;

// Output
out vec2 fUV;

void main() {
	gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);
	fUV = vTexCoords;
}
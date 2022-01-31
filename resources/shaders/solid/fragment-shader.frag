#version 330 core

// In from vert shader
in vec3 fNormal;
in vec3 fPos;
in vec2 fUV;

in mat3 TBN;

// Output
out vec4 FragColor;

// Uniforms
// uniform sampler2D diffuse1;
// uniform sampler2D specular1;
// uniform sampler2D normal1;

uniform int specularMapCount;
uniform int normalMapCount;
uniform int diffusionMapCount;

uniform vec3 cameraPos;

void main() {
   // Set fragment color
   vec4 defaultColour = vec4(0.5, 0.5, 0.5, 1.0);
   // FragColor = (int(!(diffusionMapCount == 0))*texture(diffuse1, fUV) + int(diffusionMapCount == 0)*defaultColour);
   FragColor = defaultColour;
};
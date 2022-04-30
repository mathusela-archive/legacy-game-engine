#version 330 core

// Uniforms
uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D normal1;

uniform int specularMapCount;
uniform int normalMapCount;
uniform int diffusionMapCount;

uniform mat4 world;

// In from vert shader
in vec3 fNormal;
in vec3 fPos;
in vec2 fUV;

in mat3 TBN;

// Output
layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 FragPos;
layout (location=2) out vec4 FragNormal_Specular;

void main() {
	// Color
	vec4 defaultColour = vec4(0.5, 0.5, 0.5, 1.0);
	FragColor = (int(!(diffusionMapCount == 0))*texture(diffuse1, fUV) + int(diffusionMapCount == 0)*defaultColour);

	// Position
	FragPos.xyz = fPos;

	// Normal
    FragNormal_Specular.xyz = normalize(vec3(world * vec4(int(!(normalMapCount == 0))*normalize(TBN*(texture(normal1, fUV).rgb * 2.0 -1.0)) + int(normalMapCount == 0)*normalize(fNormal), 0.0)));

	// Specular
	FragNormal_Specular.w = (int(!(specularMapCount == 0))*vec3(texture(specular1, fUV)) + int(specularMapCount == 0)*vec3(0.5)).x;
};
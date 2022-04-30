#version 330 core

#define POINT 0
#define DIRECTIONAL 1

// Settings
#define MAX_LIGHTS 25
float shininess = 32.0;
float linearFalloff = 0.09;
float quadraticFalloff = 0.032;
float specularStrength = 0.9;
float ambientStrength = 0.1;

// In from vert shader
in vec2 fUV;

// Output
out vec4 FragColor;

// Structs
struct Light {
   vec3 position;
   vec3 color;
   float power;
   int type;
};

// Uniforms
uniform sampler2D colorMap;
uniform sampler2D positionMap;
uniform sampler2D normal_specularMap;

uniform vec3 cameraPos;
uniform Light sceneLights[MAX_LIGHTS];
uniform int lightsCount;

void main() {
   vec3 lighting = vec3(0.0);

   // Calculate lighting for every source
	vec3 fPos = texture(positionMap, fUV).xyz;

   for (int i=0; i<lightsCount; i++) {
      vec3 unitNormal = texture(normal_specularMap, fUV).xyz;
      vec3 lightDir = int(sceneLights[i].type == POINT)*normalize(sceneLights[i].position - fPos) + int(sceneLights[i].type == DIRECTIONAL)*normalize(sceneLights[i].position);
      vec3 cameraDir = normalize(cameraPos - fPos);
      vec3 halfwayDir = normalize(lightDir + cameraDir);

      // Attenuation
      float distance = length(sceneLights[i].position - fPos);
      float attenuation = int(sceneLights[i].type == POINT)*(1.0 / (1.0 + linearFalloff * distance + quadraticFalloff * (distance * distance))) + int(sceneLights[i].type == DIRECTIONAL);

      // Diffuse
      float diffusePower = max(dot(unitNormal, lightDir), 0.0);
      float diffuseComponent = diffusePower * attenuation;

      // Specular
      vec3 specularMap = vec3(texture(normal_specularMap, fUV).w);
      float specularPower = pow(max(dot(unitNormal, halfwayDir), 0.0), shininess);
      vec3 specularComponent = specularStrength * specularPower * attenuation * specularMap;

      lighting += ((vec3(diffuseComponent) + specularComponent) * sceneLights[i].color) * sceneLights[i].power;
   }

	// Set fragment color
	FragColor = texture(colorMap, fUV) * vec4(lighting + vec3(ambientStrength), 1.0);
};
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
in vec3 fNormal;
in vec3 fPos;
in vec2 fUV;

in mat3 TBN;

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
uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D normal1;

uniform int specularMapCount;
uniform int normalMapCount;
uniform int diffusionMapCount;

uniform vec3 cameraPos;
uniform Light sceneLights[MAX_LIGHTS];
uniform int lightsCount;

void main() {
   vec3 lighting = vec3(0.0);

   // Calculate lighting for every source
   for (int i=0; i<lightsCount; i++) {
      vec3 unitNormal = int(!(normalMapCount == 0))*normalize(TBN*(texture(normal1, fUV).rgb * 2.0 -1.0)) + int(normalMapCount == 0)*normalize(fNormal);
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
      vec3 specularMap = int(!(specularMapCount == 0))*vec3(texture(specular1, fUV)) + int(specularMapCount == 0)*vec3(0.5);
      float specularPower = pow(max(dot(unitNormal, halfwayDir), 0.0), shininess);
      vec3 specularComponent = specularStrength * specularPower * attenuation * specularMap;

      lighting += ((vec3(diffuseComponent) + specularComponent) * sceneLights[i].color) * sceneLights[i].power;
   }

   // Set fragment color
   vec4 defaultColour = vec4(0.5, 0.5, 0.5, 1.0);
   FragColor = (int(!(diffusionMapCount == 0))*texture(diffuse1, fUV) + int(diffusionMapCount == 0)*defaultColour) * vec4(lighting + vec3(ambientStrength), 1.0);
};
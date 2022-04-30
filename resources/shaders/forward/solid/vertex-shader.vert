#version 330 core

// Vertex attributes
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;

// Vars out
out vec3 fNormal;
out vec3 fPos;
out vec2 fUV;

out mat3 TBN;

// Uniforms
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPos;

void main() {
   // Set vector position (projection)
   gl_Position = projection * view * world * vec4(vPos, 1.0);
   // Set vars
   fPos = vec3(world * vec4(vPos, 1.0));
   fNormal = vNormal;
   fUV = vUV;

   // Normal map stuff
   // http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html
   vec3 T = normalize(world * vec4(vTangent, 0.0)).xyz;
   vec3 N = normalize(world * vec4(vNormal, 0.0)).xyz;
   T = normalize(T - dot(T, N) * N);
   vec3 B = cross(T, N);

   TBN = mat3(T, B, N);
}

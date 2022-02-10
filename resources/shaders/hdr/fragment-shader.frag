#version 330 core

layout (location = 0) out vec4 fColor;
layout (location = 1) out vec4 fBright;

in vec2 fTexCoords;
in vec3 fPos;

uniform sampler2D render;

uniform float exposure;
float bloomThreshold = 7.0;

void main()
{
    vec3 hdr = texture(render, fTexCoords).rgb; 

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdr * exposure);

    fColor = vec4(mapped, 1.0);
    fBright = vec4(mapped, 1.0) * int(length(hdr) > bloomThreshold);
}  
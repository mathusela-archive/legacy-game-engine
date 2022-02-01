#version 330 core

out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPos;

uniform sampler2D render;

uniform float exposure;

void main()
{
    vec3 hdr = texture(render, fTexCoords).rgb; 

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdr * exposure);

    fColor = vec4(mapped, 1.0);
}  
#version 330 core

out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPos;

uniform sampler2D render;

void main()
{
    fColor = vec4(texture(render, fTexCoords).rgb, 1.0); 
}  
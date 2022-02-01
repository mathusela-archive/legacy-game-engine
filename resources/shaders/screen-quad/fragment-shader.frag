#version 330 core

out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPos;

uniform sampler2D render;

void main()
{
    fColor = texture(render, fTexCoords);
    // fColor = vec4(1.0, 1.0, 1.0, 1.0);
}  
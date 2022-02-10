#version 330 core

out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPos;

uniform sampler2D render;
uniform sampler2D blur;

uniform float exposure;

// lim(strength) as bloomStrength => inf := 0
float bloomStrength = 2.0;

void main()
{ 
    vec3 mapped = vec3(1.0) - exp(-texture(blur, fTexCoords).rgb * exposure/bloomStrength);
    vec3 colorOut = texture(render, fTexCoords).rgb + mapped;
    fColor = vec4(colorOut, 1.0);
}  
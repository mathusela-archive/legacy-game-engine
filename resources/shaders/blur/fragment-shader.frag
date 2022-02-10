#version 330 core

out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPos;

uniform sampler2D render;

uniform bool orientation;
float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 texOffset = 1.0 / textureSize(render, 0);
    vec3 result = texture(render, fTexCoords).rgb * weight[0]; 

    if (orientation) {
        for (int i=0; i<5; i++) {
            result += texture(render, fTexCoords + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
            result += texture(render, fTexCoords - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for (int i=0; i<5; i++) {
            result += texture(render, fTexCoords + vec2(0.0, texOffset.y * i)).rgb * weight[i];
            result += texture(render, fTexCoords - vec2(0.0, texOffset.y * i)).rgb * weight[i];
        }
    }
    fColor = vec4(result, 1.0);
}  
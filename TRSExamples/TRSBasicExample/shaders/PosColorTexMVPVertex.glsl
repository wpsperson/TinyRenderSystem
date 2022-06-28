#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vColor;
out vec2 vTex;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vColor = aColor;
    vTex = aTexture;
}
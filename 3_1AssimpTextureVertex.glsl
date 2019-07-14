//����������ɫ��
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 anormal;
layout(location = 2) in vec2 aTexCord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
out vec3 ourColor;
out vec2 ourTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourTex = aTexCord;
}
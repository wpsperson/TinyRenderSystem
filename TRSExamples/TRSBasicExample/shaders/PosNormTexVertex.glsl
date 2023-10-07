#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;
out vec3 vNorm;
out vec2 vTex;
out vec3 vFragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vFragPos = vec3(model * vec4(aPos, 1.0));
    vNorm = mat3(transpose(inverse(model))) * aNormal;
    vTex = aTex;
}
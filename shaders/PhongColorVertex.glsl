#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 3) in vec3 aColor;
out vec3 Normal;
out vec3 Color;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //Normal Matrix
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Color = aColor;
    FragPos = vec3(model * vec4(aPos, 1.0));

}
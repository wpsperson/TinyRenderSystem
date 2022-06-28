#version 330 core
out vec4 FragColor;
in vec3 vNorm;
in vec2 vTex;
uniform sampler2D texture_diffuse;
void main()
{
    vec4 color0 = texture(texture_diffuse, vTex);
    FragColor = color0;
} 
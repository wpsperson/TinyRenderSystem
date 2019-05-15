//最简单的着色器， 传入统一颜色值
#version 330 core
out vec4 FragColor;
uniform vec4 baseColor;
void main()
{
    FragColor = baseColor;
}
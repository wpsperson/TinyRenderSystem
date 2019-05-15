//两个纹理与顶点颜色混合着色器
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTex;
uniform sampler2D texture0;
uniform sampler2D texture1;
void main()
{
    vec4 color_text = mix(texture(texture0, ourTex),texture(texture1, ourTex), 0.2);
    FragColor = mix(color_text, vec4(ourColor,1.0), 0.3);
} 
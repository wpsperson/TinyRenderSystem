//两个纹理着色器
#version 330 core
out vec4 FragColor;
in vec2 ourTex;
uniform sampler2D texture_diffuse;
void main()
{
    vec4 color0 = texture(texture_diffuse, ourTex);
    //vec4 color1 = texture(texture1, ourTex);
    //vec4 mixTexture = mix(color0, color1, 0.2);
    FragColor = color0;
} 
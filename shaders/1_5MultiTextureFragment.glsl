//两个纹理着色器
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTex;
//in flat float sum;
uniform sampler2D texture0;
uniform sampler2D texture1;
void main()
{
    vec4 color0 = texture(texture0, ourTex);
    vec4 color1 = texture(texture1, ourTex);
    vec4 mixTexture = mix(color0, color1, 0.2);
    FragColor = mixTexture;
    //if (sum > 0.1)
    //    discard;
    //else
    //    FragColor = mixTexture;
    //FragColor = mix(mixTexture, vec4(ourColor,1.0), 0.3);//在两个纹理混合基础上，再加上每个顶点的颜色混合
} 
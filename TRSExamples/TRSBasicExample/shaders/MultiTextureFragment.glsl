#version 330 core
out vec4 FragColor;
in vec3 vColor;
in vec2 vTex;
uniform sampler2D texture0;
uniform sampler2D texture1;
void main()
{
    vec4 color0 = texture(texture0, vTex);
    vec4 color1 = texture(texture1, vTex);
    if(color0.a == 0.0)
    {
        FragColor = color1;
        return;
    }
    else if(color1.a== 0.0)
    {
        FragColor = color0;
        return;
    }
    vec4 mixTexture = mix(color0, color1, 0.2);
    FragColor = mixTexture;
} 
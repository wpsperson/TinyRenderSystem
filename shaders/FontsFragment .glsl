#version 460 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D ourTexture;
uniform vec4 baseColor;

void main()
{
    vec4 color = vec4(baseColor.xyz, texture(ourTexture, texCoord).r) ;
    FragColor = color;
} 
#version 460 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D ourTexture;
uniform vec3 uColor;

void main()
{
    vec4 color = vec4(uColor, texture(ourTexture, texCoord).r) ;
    FragColor = color;
} 
#version 460 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D ourTexture;
uniform vec4 baseColor;

void main()
{
    float value = texture(ourTexture, texCoord).r;
    if (value < 0.5)
    {
        discard;
    }
    vec4 color = vec4(baseColor.xyz, value * baseColor.w) ;
    FragColor = color;
} 
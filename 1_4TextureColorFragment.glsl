#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;
uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, texCoord);
    vec4 color = vec4(ourColor, 1.0);
    //FragColor = tex*0.5 + color*0.5;//颜色较淡的混合
    FragColor = color*tex;//颜色较浓的混合
} 
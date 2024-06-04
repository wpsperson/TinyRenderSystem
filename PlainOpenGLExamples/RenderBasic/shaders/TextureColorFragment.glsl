#version 330 core
out vec4 FragColor;

in vec3 vColor;
in vec2 vTex;
uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, vTex);
    vec4 color = vec4(vColor, 1.0);
    if(tex.a == 0.0)
    {
        discard;
        return;
    }
    //FragColor = tex*0.5 + color*0.5;  // light color blend
    FragColor = color * tex;            // deep color blend
} 
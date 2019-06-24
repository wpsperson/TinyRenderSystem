//����������ɫ��
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTex;
uniform sampler2D texture0;
uniform sampler2D texture1;
void main()
{
    vec4 color0 = texture(texture0, ourTex);
    vec4 color1 = texture(texture1, ourTex);
    vec4 mixTexture = mix(color0, color1, 0.2);
    FragColor = mixTexture;
    //FragColor = mix(mixTexture, vec4(ourColor,1.0), 0.3);//�����������ϻ����ϣ��ټ���ÿ���������ɫ���
} 
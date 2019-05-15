//¹âÕÕÆ¬¶Î×ÅÉ«Æ÷
#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    //FragColor = vec4(lightColor * objectColor, 1.0);

    //1 ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //2 diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(0.0, dot(norm, lightDir));
    vec3 diffuse = diff * lightColor;

    //3 specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result,1.0);

}
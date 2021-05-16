#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec4 baseColor;


void main()
{
    //1 ambient
    vec3 ambient = vec3(baseColor) * 0.2;

    //2 diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(baseColor) * 0.5;

    ////3 specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = (spec * vec3(1.0,1.0,1.0)) * 0.3;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}
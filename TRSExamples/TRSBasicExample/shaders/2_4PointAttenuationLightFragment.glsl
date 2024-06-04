#version 330 core
out vec4 FragColor;
in vec3 vNorm;
in vec3 vFragPos;
in vec2 vTex;

uniform vec3 viewPos;

struct Material {
    //vec3 ambient;
    //vec3 diffuse;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //according attenuation of point light, light intensity = 1/(Kc + Kl*d + Kq * d*d)
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;



void main()
{
    //1 ambient
    vec3 ambient = light.ambient * texture(material.diffuse, vTex).rgb;

    //2 diffuse
    vec3 norm = normalize(vNorm);
    vec3 lightDir = normalize(light.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, vTex).rgb);

    //3 specular
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 normColor = texture(material.specular, vTex).rgb;
    float average = (normColor.x + normColor.y + normColor.z) / 3.0f;
    vec3 specular = light.specular * (spec * average);
    // attenuation by distance
    float distance = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result * attenuation, 1.0);


}
#version 330 core
out vec4 FragColor;
in vec3 vNorm;
in vec3 vFragPos;
in vec2 vTex;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //according attenuation of point light, light intensity = 1/(Kc + Kl*d + Kq * d*d)
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 viewDir = normalize(viewPos - vFragPos);

    vec3 result = CalcDirLight(dirLight, vNorm, viewDir);
    // point light
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], vNorm, vFragPos, viewDir);
    // spot light
    result += CalcSpotLight(spotLight, vNorm, vFragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    //2 diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);

    //3 specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient *          texture(material.diffuse, vTex).rgb;
    vec3 diffuse = light.diffuse * diff *   texture(material.diffuse, vTex).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, vTex).rgb;
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    //3 specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation by distance
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    vec3 ambient = light.ambient *          texture(material.diffuse, vTex).rgb;
    vec3 diffuse = light.diffuse * diff *   texture(material.diffuse, vTex).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, vTex).rgb;

    vec3 result = ambient + diffuse + specular;

    return result * attenuation;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //2 diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);// from fragment to light
    float diff = max(dot(norm, lightDir), 0.0);

    //3 specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient *          texture(material.diffuse, vTex).rgb;
    vec3 diffuse = light.diffuse * diff *   texture(material.diffuse, vTex).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, vTex).rgb;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //if (theta > light.cutOff)
    //{
    //    vec3 result = ambient + diffuse + specular;
    //    FragColor = vec4(result, 1.0);
    //}
    //else
    //{
    //    vec3 result = ambient;
    //    FragColor = vec4(result, 1.0);
    //}
    vec3 result = ambient + diffuse*intensity + specular*intensity;
    return result;
}
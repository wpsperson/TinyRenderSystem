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
struct Light {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;



void main()
{
    //1 ambient
    vec3 ambient = light.ambient * texture(material.diffuse, vTex).rgb;

    //2 diffuse
    vec3 norm = normalize(vNorm);
    vec3 lightDir = normalize(light.position - vFragPos);// from fragment to light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, vTex).rgb);

    //3 specular
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 normColor = texture(material.specular, vTex).rgb;
    float average = (normColor.x + normColor.y + normColor.z) / 3.0f;
    vec3 specular = light.specular * (spec * average);

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
    FragColor = vec4(result, 1.0);

}
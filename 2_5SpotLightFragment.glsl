//材质片段着色器
#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

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
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    //2 diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);//从片段指向光源的向量
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoord).rgb);

    //3 specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 normColor = texture(material.specular, TexCoord).rgb;
    float average = (normColor.x + normColor.y + normColor.z) / 3.0f;
    vec3 specular = light.specular * (spec * average);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //if (theta > light.cutOff)
    //{
    //    // 执行光照计算
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
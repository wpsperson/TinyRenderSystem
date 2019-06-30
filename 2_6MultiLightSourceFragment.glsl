//����Ƭ����ɫ��
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
    //����˥���ĵ��Դ�Ĺ�ʽ�е�������  ��ǿ�� = 1/(Kc + Kl*d + Kq * d*d)
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
vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 Normal, vec3 FragPos, vec3 viewDir);


void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, Normal, viewDir);
    // �ڶ��׶Σ����Դ
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);
    // �����׶Σ��۹�
    result += CalcSpotLight(spotLight, Normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    //2 diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    //3 specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient *          texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff *   texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPos, vec3 viewDir)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    //3 specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //���յ��Դ����˥��
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    vec3 ambient = light.ambient *          texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff *   texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    vec3 result = ambient + diffuse + specular;

    return result * attenuation;
}


vec3 CalcSpotLight(SpotLight light, vec3 Normal, vec3 FragPos, vec3 viewDir)
{
    //2 diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);//��Ƭ��ָ���Դ������
    float diff = max(dot(norm, lightDir), 0.0);

    //3 specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient *          texture(material.diffuse, TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff *   texture(material.diffuse, TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //if (theta > light.cutOff)
    //{
    //    // ִ�й��ռ���
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
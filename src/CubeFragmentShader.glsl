#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 FragColor;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 法线
    vec3 norm = normalize(Normal);
    // 光线方向
    vec3 lightDir = normalize(light.position - FragPos);
    // 计算衰减
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 镜面反射
    // 观察者到点的朝向
    vec3 viewDir = normalize(viewPos - FragPos);
    // 观察者到点到朝向根据法向量进行反射
    vec3 reflectDir = reflect(-lightDir, norm);
    // 观察者角度越接近法向镜面效果越强
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 后面的幂指数加强镜面的衰减程度
    vec3 specular = light.specular * (spec * material.specular);

    // 环境光+漫反射光
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
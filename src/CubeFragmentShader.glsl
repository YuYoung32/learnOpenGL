#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; // 光线起点世界坐标
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 法线
    vec3 norm = normalize(Normal);
    // 光线方向
    vec3 lightDir = normalize(lightPos - FragPos);
    // 计算衰减
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    float specularStrength = 0.5;
    // 观察者到点的朝向
    vec3 viewDir = normalize(viewPos - FragPos);
    // 观察者到点到朝向根据法向量进行反射
    vec3 reflectDir = reflect(-lightDir, norm);
    // 观察者角度越接近法向镜面效果越强
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64); // 后面的幂指数加强镜面的衰减程度
    vec3 specular = specularStrength * spec * lightColor;

    // 环境光+漫反射光
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
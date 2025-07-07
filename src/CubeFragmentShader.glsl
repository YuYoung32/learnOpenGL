#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; // 光线起点世界坐标

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
    // 环境光+漫反射光
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
#version 330 core

struct Material {
// ambient和diffuse相同
    sampler2D diffuse;// 相当于一个map 从纹理坐标取diffuse值达到更加精细的控制光线, amibient通常和diffuse相同
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;// 平行光专用 点光源的光线方向是根据每个点计算的
    float cutOff;// 聚光(一种特殊的点光源) 专用
    float outerCutOff;// 聚光(一种特殊的点光源) 专用, 外切角, 用于做聚光模糊

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;// 衰减系数, 点光源专用
    float linear;// 衰减系数, 点光源专用
    float quadratic;// 衰减系数, 点光源专用
};

uniform Light light;

out vec4 FragColor;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{

    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // 法线
    vec3 norm = normalize(Normal);
    // 光线方向
    vec3 lightDir = normalize(FragPos - light.position);
    float theta = dot(lightDir, normalize(-light.position));
    if (theta > light.outerCutOff) {
        // 角度大于锥体 在聚光之外
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
        return;
    }
    // 计算衰减
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // 做差值以实现聚光模糊
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // 镜面反射
    // 观察者到点的朝向
    vec3 viewDir = normalize(viewPos - FragPos);
    // 观察者到点到朝向根据法向量进行反射
    vec3 reflectDir = reflect(-lightDir, norm);
    // 观察者角度越接近法向镜面效果越强
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);// 后面的幂指数加强镜面的衰减程度
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // 所有光受到距离限制进行衰减
    ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
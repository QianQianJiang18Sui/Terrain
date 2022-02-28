#version 330 core
in vec3 vertexColor;
in vec3 world_pos;
in vec3 viewPos;
in vec3 normal;

out vec4 fragColor;


struct DirLight
{
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);

    vec3 ambient  = light.ambient  * vertexColor;
    vec3 diffuse  = light.diffuse  * vertexColor;
    vec3 specular = light.specular * spec * vertexColor;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                 light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient  = light.ambient  * vertexColor;
    vec3 diffuse  = light.diffuse  * diff * vertexColor;
    vec3 specular = light.specular * spec * vertexColor;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main(void)
{
    DirLight light;
    light.direction = normalize(-viewPos);
    light.color = vec3(1.0, 1.0, 1.0);
    light.ambient = vec3(0.2, 0.2, 0.2);
    light.diffuse = vec3(0.5, 0.5, 0.5);
    light.specular = vec3(1.0, 1.0, 1.0);

    PointLight pt_light;
    pt_light.position = viewPos;
    pt_light.ambient = vec3(0.05, 0.05, 0.05);
    pt_light.diffuse = vec3(0.8f, 0.8f, 0.8f);
    pt_light.specular = vec3(1.0, 1.0, 1.0);
    pt_light.constant = 1.0;
    pt_light.linear = 0.09;
    pt_light.quadratic = 0.032;

    Material material;
    material.shininess = 0.6;
    material.ambient   = vec3(0.45f,    0.45f,      0.45f);
    material.diffuse   = vec3(0.7f,     0.7f,       0.7f);
    material.specular  = vec3(0.296648, 0.296648f,  0.296648f);


    vec3 viewDir = normalize(viewPos - world_pos);
    vec3 color = CalculateDirectionalLight(light, normal, viewDir, material);
    color += CalcPointLight(pt_light, normal, world_pos, viewDir, material);

    fragColor = vec4(color, 1.0);


//    bool enableLight = false;
//    if (enableLight)
//    {
//        vec3 normal = vec3(0.0, 1.0, 0.0);
//        vec3 LightPos1 = vec3(-20, 30, 0);
//        vec3 LightColor = vec3(1.0, 1.0, 1.0);
//        vec3 Ambient = vec3(1.0, 1.0, 1.0);
//        float ConstantAttenuation  = 0.005;//衰减系数
//        float LinearAttenuation    = 0.005;
//        float QuadraticAttenuation = 0.01;
//        float Shininess            = .005;
//        vec3  EyeDirection         = vec3(.0, 0, 1.0);
//        float Strength             = 0.2;

//        vec3 normal1 = normalize(normal);
//        vec3 lightDirection = LightPos1 - vec3(world_pos);//点指向光源的方向
//        float lightDistance = length(lightDirection);//点到光源的距离

//        lightDirection = lightDirection / lightDistance;//方向单位化

//        float attenuation = 1.0 / (ConstantAttenuation +
//                                   LinearAttenuation * lightDistance +
//                                   QuadraticAttenuation * lightDistance * lightDistance);

//        vec3 halfVector  = /*lightDirection;*/normalize(lightDirection + EyeDirection);

//        float diffuse = max(0.0, dot(normal1, lightDirection));
//        float specular = max(0.0, dot(normal1, halfVector));

//        if(diffuse == 0.0)
//            specular = 0.0;
//        else
//            specular = pow(specular, Shininess) * Strength;

//        vec3 scatteredLight = Ambient + LightColor * diffuse * attenuation;
//        vec3 reflectedLight = LightColor * specular * attenuation;

//        vec3 rgb = min(color.rgb * scatteredLight + reflectedLight, vec3(1.0));
//        fragColor = vec4(rgb, 1.0);
//    }
//    else
//    {
//        fragColor = vec4(color, 1.0);
//    }
}

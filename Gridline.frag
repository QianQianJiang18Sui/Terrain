#version 330 core

in vec3 color;
in vec3 world_pos;
//in vec3 textureCoord;
//in vec3 normal;

//uniform sampler2D texture0;


void main(void)
{
    bool enableLight = false;
    if (enableLight)
    {
        vec3 normal = vec3(0.0, 0.0, 1.0);
        vec3 LightPos1 = vec3(0, 3, 0);
        vec3 LightColor = vec3(1.0, 1.0, 1.0);
        vec3 Ambient = vec3(0.0, 0.0, 0.0);
        float ConstantAttenuation  = 0.005;//衰减系数
        float LinearAttenuation    = 0.005;
        float QuadraticAttenuation = 0.01;
        float Shininess            = .005;
        vec3  EyeDirection         = vec3(.0, 0, 1.0);
        float Strength             = 0.2;

        vec3 normal1 = normalize(normal);
        vec3 lightDirection = LightPos1 - vec3(world_pos);//点指向光源的方向
        float lightDistance = length(lightDirection);//点到光源的距离

        lightDirection = lightDirection / lightDistance;//方向单位化

        float attenuation = 1.0 / (ConstantAttenuation +
                                   LinearAttenuation * lightDistance +
                                   QuadraticAttenuation * lightDistance * lightDistance);

        vec3 halfVector  = /*lightDirection;*/normalize(lightDirection + EyeDirection);

        float diffuse = max(0.0, dot(normal1, lightDirection));
        float specular = max(0.0, dot(normal1, halfVector));

        if(diffuse == 0.0)
            specular = 0.0;
        else
            specular = pow(specular, Shininess) * Strength;

        vec3 scatteredLight = Ambient + LightColor * diffuse * attenuation;
        vec3 reflectedLight = LightColor * specular * attenuation;

        vec3 rgb = min(color.rgb * scatteredLight + reflectedLight, vec3(1.0));

        gl_FragColor = vec4(rgb, 1.0);
    }
    else
    {
        gl_FragColor = vec4(color, 1.0);
    }
    //gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}

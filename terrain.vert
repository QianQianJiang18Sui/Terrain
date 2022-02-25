#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPos;
uniform vec3 modelColor;
uniform int  bEnableColorArray;

out vec3 vertexColor;
out vec3 world_pos;
out vec3 viewPos;
out vec3 normal;

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    if (bEnableColorArray == 1)
        vertexColor =  aColor;
    else
        vertexColor = modelColor;

    world_pos = vec3(model * vec4(aPos, 1.0));
    normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    viewPos = cameraPos;
}

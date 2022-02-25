#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 4) in vec3 aTextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 modelColor;
uniform vec3 cameraPos;

out vec3 vertexColor;
out vec3 normal;
out vec3 world_pos;
out vec3 viewPos;

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    vertexColor = modelColor;
    world_pos = vec3(model * vec4(aPos, 1.0));
    viewPos = cameraPos;

    //    color = aColor;
    //    textureCoord = aTextureCoord;
}

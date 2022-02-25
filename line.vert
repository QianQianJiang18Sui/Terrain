#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec3 aNormal;
//layout (location = 4) in vec3 aTextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 modelColor;

out vec3 color;
//out vec3 textureCoord;
//out vec3 normal;
out vec3 world_pos;

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
//    color = aColor;
//    normal = aNormal;
//    textureCoord = aTextureCoord;
    color = modelColor;
    world_pos = vec3(model * vec4(aPos, 1.0));
}

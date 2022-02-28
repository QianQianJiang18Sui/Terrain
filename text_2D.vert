#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPos;

out vec3 world_pos;
out vec3 viewPos;
out vec2 TexCoord;

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    world_pos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTextCoord;
    viewPos = cameraPos;
}

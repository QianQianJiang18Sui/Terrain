#version 330 core
layout (location = 0) in vec3 inVertexPosition;

//layout (location = 1) in vec3 textCoord;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform vec4 modelColor;

out vec4 outColor;

void main(void)
{
    gl_Position = projection * view * model * vec4(inVertexPosition, 1.0);
    outColor = modelColor;
}

#version 330 core

in vec3 world_pos;
in vec3 viewPos;
in vec2 TexCoord;

uniform sampler2D qt_Texture0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, TexCoord);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

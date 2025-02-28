#version 430

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 textCoords;

out vec2 f_textCoord;

void main()
{
    gl_Position = vec4(pos,0.0,1.0);
    f_textCoord = textCoords;
}

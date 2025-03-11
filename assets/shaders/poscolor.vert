#version 430

layout (location = 0) in vec2 vert_pos;
layout (location = 1) in vec4 vert_color;

uniform mat4 MVP;

out vec4 f_color;

void main() {
    gl_Position = MVP * vert_color;//vec4(vert_pos,0.0,1.0);
    f_color = vert_color;
}
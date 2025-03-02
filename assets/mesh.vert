#version 430

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_norm;
layout (location = 2) in vec2 vert_textCoord;

uniform mat4 MVP;

out vec3 f_pos;
out vec3 f_norm;
out vec2 f_textCoord;

void main() {
    gl_Position = MVP * vec4(vert_pos,1.0);//MVP * vec4(vert_textCoord,0.0,1.0);
    f_pos = vert_pos;
    f_norm = vert_norm;
    f_textCoord = vert_textCoord;
}
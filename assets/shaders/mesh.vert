#version 430

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_norm;
layout (location = 2) in vec2 vert_textCoord;

uniform mat4 MVP;

out VS_OUT {
    vec3 f_pos;
    vec3 f_norm;
    vec2 f_textCoord;
} vs_out;

void main() {
    gl_Position = MVP * vec4(vert_pos,1.0);//MVP * vec4(vert_textCoord,0.0,1.0);
    vs_out.f_pos = vert_pos;
    vs_out.f_norm = vert_norm;
    vs_out.f_textCoord = vert_textCoord;
}
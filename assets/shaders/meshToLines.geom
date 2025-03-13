#version 430

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in GEOM_IN {
    vec3 f_pos;
    vec3 f_norm;
    vec2 f_textCoord;
} geom_in[];

out GEOM_OUT {
    vec3 f_pos;
    vec3 f_norm;
    vec2 f_textCoord;
} geom_out;

void main()
{
    geom_out.f_norm = normalize(geom_in[0].f_norm+geom_in[1].f_norm+geom_in[2].f_norm); // technically not necessary
    
    // 0
    geom_out.f_pos = geom_in[0].f_pos;
    geom_out.f_textCoord = geom_in[0].f_textCoord;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    // 1
    geom_out.f_pos = geom_in[1].f_pos;
    geom_out.f_textCoord = geom_in[1].f_textCoord;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    
    // 2
    geom_out.f_pos = geom_in[2].f_pos;
    geom_out.f_textCoord = geom_in[2].f_textCoord;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    
    // 3 % 3
    geom_out.f_pos = geom_in[0].f_pos;
    geom_out.f_textCoord = geom_in[0].f_textCoord;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
}
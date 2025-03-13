#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 f_pos;
    vec3 f_norm;
    vec2 f_textCoord;
} geom_in[];

out VS_OUT {
    vec3 f_pos;
    vec3 f_norm;
    vec2 f_textCoord;
} geom_out;

void main()
{
    //geom_out.f_norm = normalize(geom_in[0].f_norm + geom_in[1].f_norm + geom_in[2].f_norm);
    
    // /!\ only works with **ClockWise** faces
    geom_out.f_norm = -normalize(cross(geom_in[1].f_pos - geom_in[0].f_pos,geom_in[2].f_pos - geom_in[0].f_pos));

    for(int i = 0; i<3;++i)
    {
        geom_out.f_pos = geom_in[i].f_pos;
        geom_out.f_textCoord = geom_in[i].f_textCoord;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
}
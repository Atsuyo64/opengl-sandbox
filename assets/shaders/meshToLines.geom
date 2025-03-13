#version 430

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

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
    //geom_out.f_norm = normalize(geom_in[0].f_norm + geom_in[1].f_norm + geom_in[2].f_norm); // technically not necessary
    
    for(int i = 0; i<4;++i)
    {
        geom_out.f_norm = geom_in[i%3].f_norm;
        geom_out.f_pos = geom_in[i%3].f_pos;
        geom_out.f_textCoord = geom_in[i%3].f_textCoord;
        gl_Position = gl_in[i%3].gl_Position;
        EmitVertex();
    }
}
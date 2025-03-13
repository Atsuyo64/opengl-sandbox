#version 430

in VS_OUT {
    vec3 f_pos;
    vec3 f_norm;
    vec2 f_textCoord;
} frag_in;

out vec4 Fragment_Color;
//layout (location = 0) out vec4 Fragment_Color;

uniform int lim;

void main()
{
    Fragment_Color = vec4(frag_in.f_norm,1.0); //vec4(f_textCoord,0.0,1.0);
}
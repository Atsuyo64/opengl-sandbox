#version 430

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_textCoord;

out vec4 Fragment_Color;
//layout (location = 0) out vec4 Fragment_Color;

uniform int lim;

void main()
{
    Fragment_Color = vec4(pow(f_norm,vec3(1.0)),1.0); //vec4(1.0);
}
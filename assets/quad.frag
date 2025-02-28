#version 430
in vec2 f_textCoord;

uniform sampler2D rendered_texture;

out vec4 Fragment_Color;

void main()
{
    vec3 color = texture(rendered_texture,f_textCoord).xyz;
    Fragment_Color = vec4(length(color) < 0.99 ? color.x : 1.0 ,color.y,color.z,1.0);
}
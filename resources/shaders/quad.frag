#version 450

in  layout(location = 0) vec2 in_textureCoord;

out layout(location = 0) vec4 out_color;

uniform sampler2D u_texture;
uniform vec4 u_color;


void main()
{
    out_color = u_color * texture(u_texture, in_textureCoord);
}

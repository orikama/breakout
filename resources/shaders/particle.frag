#version 450

layout(location = 0) in vec2 in_textureCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D u_texture;
uniform vec4 u_color;


void main()
{
    out_color = texture(u_texture, in_textureCoord) * u_color;
}

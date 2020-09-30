#version 450

layout(location = 0) in vec2 in_textureCoord;
layout(location = 1) in vec4 in_particleColor;

layout(location = 0) out vec4 out_color;

uniform sampler2D u_texture;


void main()
{
    out_color = texture(u_texture, in_textureCoord) * in_particleColor;
}

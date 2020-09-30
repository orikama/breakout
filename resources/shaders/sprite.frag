#version 450

layout(location = 0) in vec2 in_textureCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D u_texture;
uniform vec3 u_spriteColor;


void main()
{
    out_color = vec4(u_spriteColor, 1.0) * texture(u_texture, in_textureCoord);
}

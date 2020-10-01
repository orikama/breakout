#version 450

layout(location = 0) in vec2 in_textureCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D u_textTexture;
uniform vec3 u_textColor;


void main()
{    
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_textTexture, in_textureCoord).r);
    out_color = vec4(u_textColor, 1.0f) * sampled;
}

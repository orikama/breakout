#version 450

layout(location = 0) in vec4 in_vertex; // <vec2 position, vec2 textCoords>

layout(location = 0) out vec2 out_textureCoord;

uniform mat4 u_projection;


void main()
{
    out_textureCoord = in_vertex.zw;
    gl_Position = u_projection * vec4(in_vertex.xy, 0.0f, 1.0f);
}

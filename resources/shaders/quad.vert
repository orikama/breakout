#version 450

in  layout(location = 0) vec4 in_vertex; // <vec2 position, vec2 textCoords>

out layout(location = 0) vec2 out_textureCoord;

uniform mat4 u_projection;
uniform mat4 u_model;


void main()
{
    out_textureCoord = in_vertex.zw;
    gl_Position = u_projection * u_model * vec4(in_vertex.xy, 0.0, 1.0);
}

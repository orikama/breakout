#version 450

layout(location = 0) in vec4 in_vertex; // <vec2 position, vec2 textCoords>

layout(location = 0) out vec2 out_textureCoord;

uniform float u_time;
uniform bool u_chaos;
uniform bool u_confuse;
uniform bool u_shake;


void main()
{
    vec2 position = in_vertex.xy;
    vec2 textCoords = in_vertex.zw;

    if (u_chaos) {
        float strength = 0.3f;
        out_textureCoord = textCoords + strength * vec2(sin(u_time), cos(u_time));
    }
    else if (u_confuse) {
        out_textureCoord = 1.0f - textCoords;
    }
    else {
        out_textureCoord = textCoords;
    }

    if (u_shake) {
        float strength = 0.01f;
        position += strength * vec2(cos(u_time * 10), cos(u_time * 15));
    }

    gl_Position = vec4(position, 0.0f, 1.0f);
}

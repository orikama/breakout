#version 450

layout(location = 0) in vec2 in_textureCoord;

layout(location = 0) out vec4 out_color;

uniform sampler2D u_scene;
uniform vec2 u_offsets[9];
uniform int u_edgeKernel[9];
uniform float u_blurKernel[9];

uniform bool u_chaos;
uniform bool u_confuse;
uniform bool u_shake;


void main()
{
    out_color = vec4(0.0f);
    vec3 sampleK[9];

    if (u_chaos || u_shake) {
        for (int i = 0; i < 9; ++i) {
            sampleK[i] = vec3(texture(u_scene, in_textureCoord.st + u_offsets[i]));
        }
    }

    if (u_chaos) {
        for (int i = 0; i < 9; ++i) {
            out_color += vec4(sampleK[i] * u_edgeKernel[i], 0.0f);
        }
        out_color.a = 1.0f;
    }
    else if (u_confuse) {
        out_color = vec4(1.0f - texture(u_scene, in_textureCoord).rgb, 1.0f);
    }
    else if (u_shake) {
        for (int i = 0; i < 9; ++i) {
            out_color += vec4(sampleK[i] * u_blurKernel[i], 0.0f);
        }
        out_color.a = 1.0f;
    }
    else {
        out_color = texture(u_scene, in_textureCoord);
    }
}

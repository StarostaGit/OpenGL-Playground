#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform vec4 u_Color;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main() {
    vec4 tex1 = texture(u_Texture1, v_TexCoords);
    vec4 tex2 = texture(u_Texture2, v_TexCoords);
    vec4 texColor = mix(tex1, tex2, 1 - (0.7 * tex1.a));
    color = texColor;
}
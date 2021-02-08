#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (location = 0) out vec3 v_FragmentPosition;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec2 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    v_FragmentPosition = vec3(u_Model * vec4(position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * normal;
    v_TexCoords = texCoords;
}
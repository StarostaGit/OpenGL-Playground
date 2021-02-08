#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) out vec4 color;

in vec3 v_FragmentPosition;
in vec3 v_Normal;

uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_CameraPosition;

void main() {
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    vec3 norm = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_FragmentPosition);

    float diffCoverage = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = u_Light.diffuse * (diffCoverage * u_Material.diffuse);

    vec3 viewingDirection = normalize(u_CameraPosition - v_FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float specularIntensity = pow(max(dot(viewingDirection, reflectionDirection), 0.0), u_Material.shine);
    vec3 specular = u_Light.specular * (specularIntensity * u_Material.specular);

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
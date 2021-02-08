#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D shine;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 v_FragmentPosition;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoords;

uniform Material u_Material;
uniform DirectionalLight u_Light;
uniform vec3 u_CameraPosition;

void main() {
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));

    vec3 norm = normalize(v_Normal);
    vec3 lightDirection = normalize(-u_Light.direction);

    float diffCoverage = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = u_Light.diffuse * diffCoverage * vec3(texture(u_Material.diffuse, v_TexCoords));

    vec3 viewingDirection = normalize(u_CameraPosition - v_FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float shine = 255.0 * texture(u_Material.shine, v_TexCoords).r;
    float specularIntensity = pow(max(dot(viewingDirection, reflectionDirection), 0.0), shine);
    vec3 specular = u_Light.specular * specularIntensity * vec3(texture(u_Material.specular, v_TexCoords));

    vec3 result = ambient + diffuse + specular;

    vec3 emission = vec3(texture(u_Material.emission, v_TexCoords));
    if (emission != vec3(0.0))
    result = result + emission;

    color = vec4(result, 1.0);
}
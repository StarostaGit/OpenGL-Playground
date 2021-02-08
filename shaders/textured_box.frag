#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D shine;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float innerCutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 v_FragmentPosition;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoords;

uniform Material u_Material;
uniform Spotlight u_Spotlight;
uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform vec3 u_CameraPosition;

vec3 getPointLight(PointLight light, vec3 norm, vec3 position, vec3 viewDir);
vec3 getDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir);
vec3 getSpotlight(Spotlight light, vec3 norm, vec3 position, vec3 viewDir);

void main() {
    vec3 norm = normalize(v_Normal);
    vec3 viewingDirection = normalize(u_CameraPosition - v_FragmentPosition);

    vec3 result = getDirectionalLight(u_DirLight, norm, viewingDirection);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += getPointLight(u_PointLights[i], norm, v_FragmentPosition, viewingDirection);
    result += getSpotlight(u_Spotlight, norm, v_FragmentPosition, viewingDirection);

    vec3 emission = vec3(texture(u_Material.emission, v_TexCoords));
    if (emission != vec3(0.0))
        result = result + emission;

    color = vec4(result, 1.0);
}

vec3 getPointLight(PointLight light, vec3 norm, vec3 position, vec3 viewDir) {
    vec3 lightDirection = normalize(light.position - position);

    float diffCoverage = max(dot(norm, lightDirection), 0.0);

    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float shine = 255.0 * texture(u_Material.shine, v_TexCoords).r;
    float specularIntensity = pow(max(dot(viewDir, reflectionDirection), 0.0), shine);

    float distance = length(light.position - position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diffCoverage * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * specularIntensity * vec3(texture(u_Material.specular, v_TexCoords));

    return attenuation * (ambient + diffuse + specular);
}

vec3 getDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDirection = normalize(-light.direction);

    float diffCoverage = max(dot(norm, lightDirection), 0.0);

    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float shine = 255.0 * texture(u_Material.shine, v_TexCoords).r;
    float specularIntensity = pow(max(dot(viewDir, reflectionDirection), 0.0), shine);

    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diffCoverage * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * specularIntensity * vec3(texture(u_Material.specular, v_TexCoords));

    return ambient + diffuse + specular;
}

vec3 getSpotlight(Spotlight light, vec3 norm, vec3 position, vec3 viewDir) {
    vec3 lightDirection = normalize(light.position - v_FragmentPosition);

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float diffCoverage = max(dot(norm, lightDirection), 0.0);

    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float shine = 255.0 * texture(u_Material.shine, v_TexCoords).r;
    float specularIntensity = pow(max(dot(viewDir, reflectionDirection), 0.0), shine);

    float distance = length(light.position - position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diffCoverage * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * specularIntensity * vec3(texture(u_Material.specular, v_TexCoords));

    return intensity * attenuation * (ambient + diffuse + specular);
}
#ifndef PLAYGROUND_LIGHT_H
#define PLAYGROUND_LIGHT_H

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float innerCutoff;
    float outerCutoff;

    float constant = 1.0f;
    float linear = 0.0f;
    float quadratic = 0.0f;
};

#endif //PLAYGROUND_LIGHT_H

#ifndef PLAYGROUND_MATERIAL_H
#define PLAYGROUND_MATERIAL_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct SimpleMaterial {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shine;
};

struct Material {
    int diffuse;
    int specular;
    int emission;
    int shine;
};

const SimpleMaterial& getSimpleMaterial(const std::string &name);
std::vector<const char*> getSimpleMaterialsList();

#endif //PLAYGROUND_MATERIAL_H

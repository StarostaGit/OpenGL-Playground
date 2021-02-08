#include "Material.h"
#include <unordered_map>
#include <iostream>

const std::unordered_map<std::string, SimpleMaterial>& materialMap() {
    static const std::unordered_map<std::string, SimpleMaterial> map {
            {"emerald", {
                                .ambient = { 0.0215f, 0.1745f, 0.0215f },
                                .diffuse = { 0.07568f, 0.61424f, 0.07568f },
                                .specular = { 0.633f, 0.727811f, 0.633f },
                                .shine = 0.6f * 128.0f
                        }},
            {"yellow rubber", {
                                .ambient = { 0.05f, 0.05f, 0.0f },
                                .diffuse = { 0.5f, 0.5f, 0.4f },
                                .specular = { 0.7f, 0.7f, 0.04f },
                                .shine = 0.078125f * 128.0f
                        }},
            {"silver", {
                                .ambient = { 0.19225f, 0.19225f, 0.19225f },
                                .diffuse = { 0.50754f, 0.50754f, 0.50754f },
                                .specular = { 0.508273f, 0.508273f, 0.508273f },
                                .shine = 0.4f * 128.0f
                        }},
            {"default", {
                                .ambient = glm::vec3 { 1.0f, 0.5f, 0.31f },
                                .diffuse = glm::vec3 { 1.0f, 0.5f, 0.31f },
                                .specular = glm::vec3 { 0.5f, 0.5f, 0.5f },
                                .shine = 32.0f
                        }}
    };

    return map;
}

const SimpleMaterial& getSimpleMaterial(const std::string &name) {
    return materialMap().at(name);
}

std::vector<const char*> getSimpleMaterialsList() {
    const auto &map = materialMap();

    std::vector<const char*> materials {};
    for (const auto &[key, val] : map)
        materials.push_back(key.c_str());

    return std::move(materials);
}
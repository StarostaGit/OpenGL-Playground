#ifndef PLAYGROUND_MULTIPLELIGHTSSCENE_H
#define PLAYGROUND_MULTIPLELIGHTSSCENE_H

#include "Scene.h"
#include "shapes.h"

class MultipleLightsScene : Scene {
public:
    explicit MultipleLightsScene(GLFWwindow *window) : window(window) {
        lightSource = shapes::sphere(0.5f, 36, 18);
        box.indices = shapes::generateIndices(36);
    }

    std::shared_ptr<Camera> getCamera(float width, float height) override {
        camera = std::make_shared<Camera>(width, height);

        camera->setDefaults(
                glm::vec3 { 1.0f, 1.68f, 3.54f },
                -101.0f,
                -25.3f,
                45.0f
        );

        camera->reset();
        return camera;
    }

    void setup() override {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        box.va = std::make_shared<VertexArray>();
        box.vb = std::make_shared<VertexBuffer>(box.vertices.data(), box.vertices.size() * 4);

        box.layout = std::make_shared<VertexBufferLayout>();
        box.layout->push(GL_FLOAT, 3);
        box.layout->push(GL_FLOAT, 3);
        box.layout->push(GL_FLOAT, 2);
        box.va->addBuffer(*box.vb, *box.layout);

        box.ib = std::make_shared<IndexBuffer>(box.indices.data(), box.indices.size() );

        cubeShader = std::make_unique<Shader>("../shaders/material.vert", "../shaders/textured_box.frag");
        cubeShader->bind();

        texDiffuse = std::make_unique<Texture>("../res/textures/container/container.png", material.diffuse);
        texDiffuse->bind(material.diffuse);
        texSpecular = std::make_unique<Texture>("../res/textures/container/container_specular.png", material.specular);
        texSpecular->bind(material.specular);
        texEmission = std::make_unique<Texture>("../res/textures/black.png", material.emission);
        texEmission->bind(material.emission);
        texShine = std::make_unique<Texture>("../res/textures/container/container_shine.png", material.shine);
        texShine->bind(material.shine);
        cubeShader->setMaterial(material);

        lightSource.va = std::make_shared<VertexArray>();
        lightSource.vb = std::make_shared<VertexBuffer>(lightSource.vertices.data(), lightSource.vertices.size() * 4);

        lightSource.layout = std::make_shared<VertexBufferLayout>();
        lightSource.layout->push(GL_FLOAT, 3);
        lightSource.va->addBuffer(*lightSource.vb, *lightSource.layout);

        lightSource.ib = std::make_shared<IndexBuffer>(lightSource.indices.data(), lightSource.indices.size() );


        lightSourceShader = std::make_unique<Shader>("../shaders/basic.vert", "../shaders/light_source.frag");
    }

    void draw() override {
        processInput();

        Renderer::get().setClearColor(skylight.diffuse.r, skylight.diffuse.g, skylight.diffuse.b, 1.0f);

        flashlight.position = camera->getPosition();
        flashlight.direction = camera->getFront();

        std::vector<glm::mat4> transforms;
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * i + rotation;
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            transform = glm::scale(transform, scale);
            transforms.push_back(transform);
        }
        cubeShader->setUniform3f("u_CameraPosition", camera->getPosition());

        cubeShader->setLight("u_DirLight", skylight);

        for (int i = 0; i < lamps.size(); i++) {
            auto light = lamps.at(i).first;
            light.position = light.position;
            if (!lamps.at(i).second)
                light.diffuse = light.specular = glm::vec3 { 0.0f };
            cubeShader->setLight("u_PointLights[" + std::to_string(i) + "]", light);
        }

        if (flashlightOn)
            cubeShader->setLight("u_Spotlight", flashlight);
        else {
            auto dummy = flashlight;
            dummy.innerCutoff = 1.0f; dummy.outerCutoff = 1.0f;
            cubeShader->setLight("u_Spotlight", dummy);
        }

        Renderer::get().draw(*box.va, *box.ib, *cubeShader, transforms);

        for (const auto &[lamp, visible] : lamps) {
            if (!visible)
                continue;

            glm::mat4 transform = glm::mat4 { 1.0f };
            transform = glm::translate(transform, lamp.position);
            transform = glm::scale(transform, glm::vec3(0.2f));
            lightSourceShader->setUniform3f("u_Color", lamp.diffuse);
            Renderer::get().draw(*lightSource.va, *lightSource.ib, *lightSourceShader, transform);
        }
    }

    void showSceneControlsWindow() override {
        ImGui::Begin("Scene Controls");
        ImGui::SliderFloat3("Scale", &scale.x, 0.0f, 5.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset Scale"))
            scale.x = scale.y = scale.z = 1.0f;
        ImGui::SliderFloat("Rotation", &rotation, -360.0f, 360.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset Rotation"))
            rotation = 0.0f;

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        for (int i = 0; i < lamps.size(); i++) {
            auto label = "Lamp " + std::to_string(i);
            auto &lamp = lamps[i].first;
            if (ImGui::ColorEdit3(label.c_str(), &lamp.diffuse.r)) {
                lamp.specular = 1.67f * lamp.diffuse;
            }
            ImGui::SameLine();
            label = "visible " + std::to_string(i);
            ImGui::Checkbox(label.c_str(), &lamps[i].second);
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        if (ImGui::ColorEdit3("Sky Color", &skylight.diffuse.r)) {
            skylight.ambient = 0.2f * skylight.diffuse;
            skylight.specular = 2.0f * skylight.diffuse;
        }

        ImGui::End();
    }

private:
    std::shared_ptr<Camera> camera = nullptr;
    GLFWwindow *window;

    std::unique_ptr<Shader> cubeShader, lightSourceShader;
    std::unique_ptr<Texture> texDiffuse, texSpecular, texEmission, texShine;

    DirectionalLight skylight = {
            .direction = glm::vec3 { -0.2f, -1.0f, -0.3f },
            .ambient = glm::vec3 { 0.1f, 0.1f, 0.1f },
            .diffuse = glm::vec3 { 0.5f, 0.5f, 0.5f },
            .specular = glm::vec3 { 1.0f, 1.0f, 1.0f }
    };

    std::vector<std::pair<PointLight, bool>> lamps = {
            {{
                     .position = glm::vec3 { 0.7f,  0.2f,  2.0f },
                     .ambient = glm::vec3 { 0.0f, 0.0f, 0.0f },
                     .diffuse = glm::vec3 { 0.3f, 0.3f, 0.3f },
                     .specular = glm::vec3 { 0.5f, 0.5f, 0.5f },
                     .constant = 1.0f,
                     .linear = 0.09f,
                     .quadratic = 0.032f
             }, true},
            {{
                     .position = glm::vec3 { 2.3f, -3.3f, -4.0f },
                     .ambient = glm::vec3 { 0.0f, 0.0f, 0.0f },
                     .diffuse = glm::vec3 { 0.3f, 0.3f, 0.3f },
                     .specular = glm::vec3 { 0.5f, 0.5f, 0.5f },
                     .constant = 1.0f,
                     .linear = 0.09f,
                     .quadratic = 0.032f
             }, true},
            {{
                     .position = glm::vec3 { -4.0f,  2.0f, -12.0f },
                     .ambient = glm::vec3 { 0.0f, 0.0f, 0.0f },
                     .diffuse = glm::vec3 { 0.3f, 0.3f, 0.3f },
                     .specular = glm::vec3 { 0.5f, 0.5f, 0.5f },
                     .constant = 1.0f,
                     .linear = 0.09f,
                     .quadratic = 0.032f
             }, true},
            {{
                     .position = glm::vec3 { 0.0f,  0.0f, -3.0f },
                     .ambient = glm::vec3 { 0.0f, 0.0f, 0.0f },
                     .diffuse = glm::vec3 { 0.3f, 0.3f, 0.3f },
                     .specular = glm::vec3 { 0.5f, 0.5f, 0.5f },
                     .constant = 1.0f,
                     .linear = 0.09f,
                     .quadratic = 0.032f
             }, true},
    };

    Spotlight flashlight = {
            .position = glm::vec3 { 0.0f },
            .direction = glm::vec3 { 0.0f },
            .ambient = glm::vec3 { 0.0f, 0.0f, 0.0f },
            .diffuse = glm::vec3 { 0.5f, 0.5f, 0.5f },
            .specular = glm::vec3 { 1.0f, 1.0f, 1.0f },
            .innerCutoff = glm::cos(glm::radians(12.5f)),
            .outerCutoff = glm::cos(glm::radians(17.5f)),
            .constant = 1.0f,
            .linear = 0.045,
            .quadratic = 0.0075
    };

    Material material = {
            .diffuse = 0,
            .specular = 1,
            .emission = 2,
            .shine = 3
    };

    float rotation = 0.0f;
    glm::vec3 scale = glm::vec3 { 1.0f };

    bool flashlightOn = false;

    shapes::Shape lightSource;

    shapes::Shape box = {
            .vertices = {
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
            },
    };

    glm::vec3 cubePositions[10] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    void processInput() {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            flashlightOn = !flashlightOn;
        }
    }
};

#endif //PLAYGROUND_MULTIPLELIGHTSSCENE_H

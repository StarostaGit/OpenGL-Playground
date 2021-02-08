#ifndef PLAYGROUND_SIMPLELIGHTNINGSCENE_H
#define PLAYGROUND_SIMPLELIGHTNINGSCENE_H

#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

class SimpleLightningScene : public Scene {
public:
    SimpleLightningScene() : materials(getSimpleMaterialsList()) {
        for (unsigned int i = 0; i < 36; i++) indices[i] = i;
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

        va = std::make_unique<VertexArray>();
        vb = std::make_unique<VertexBuffer>(vertices, sizeof vertices);

        layout = std::make_unique<VertexBufferLayout>();
        layout->push(GL_FLOAT, 3);
        layout->push(GL_FLOAT, 3);
        va->addBuffer(*vb, *layout);

        ib = std::make_unique<IndexBuffer>(indices, 36 );

        cubeShader = std::make_unique<Shader>("../shaders/basic.vert", "../shaders/solid_color_box.frag");
        lightSourceShader = std::make_unique<Shader>("../shaders/basic.vert", "../shaders/light_source.frag");

        lightSourceShader->unbind();
        cubeShader->unbind();
        vb->unbind();
        va->unbind();
        ib->unbind();

        Renderer::get().setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    }

    void draw() override {
        glm::mat4 transform { 1.0f };
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(1.0f, 0.3f, 0.5f));
        transform = glm::scale(transform, scale);
        cubeShader->setUniform3f("u_CameraPosition", camera->getPosition());
        cubeShader->setMaterial(material);
        cubeShader->setLight(light);
        Renderer::get().draw(*va, *ib, *cubeShader, transform);

        transform = glm::mat4 { 1.0f };
        transform = glm::translate(transform, light.direction);
        transform = glm::scale(transform, glm::vec3(0.2f));
        lightSourceShader->setUniform3f("u_Color", light.diffuse);
        Renderer::get().draw(*va, *ib, *lightSourceShader, transform);
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

        if (ImGui::Combo("Box Material", &currentMaterial, materials.data(), materials.size())) {
            material = getSimpleMaterial(materials[currentMaterial]);
        };

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        ImGui::ColorEdit3("Light Source Ambient", &light.ambient.x);
        ImGui::ColorEdit3("Light Source Diffuse", &light.diffuse.x);
        ImGui::ColorEdit3("Light Source Specular", &light.specular.x);
        ImGui::End();
    }

private:
    std::shared_ptr<Camera> camera = nullptr;

    std::unique_ptr<VertexArray> va;
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<IndexBuffer> ib;
    std::unique_ptr<VertexBufferLayout> layout;
    std::unique_ptr<Shader> cubeShader, lightSourceShader;

    DirectionalLight light = {
            .direction = glm::vec3 { 1.2f, 1.0f, 2.0f },
            .ambient = glm::vec3 { 0.2f, 0.2f, 0.2f },
            .diffuse = glm::vec3 { 0.5f, 0.5f, 0.5f },
            .specular = glm::vec3 { 1.0f, 1.0f, 1.0f }
    };

    int currentMaterial;
    SimpleMaterial material = {
            .ambient = glm::vec3 { 1.0f, 0.5f, 0.31f },
            .diffuse = glm::vec3 { 1.0f, 0.5f, 0.31f },
            .specular = glm::vec3 { 0.5f, 0.5f, 0.5f },
            .shine = 32.0f
    };

    float rotation = 0.0f;
    glm::vec3 scale = glm::vec3 { 1.0f };

    unsigned int indices[36];

    float vertices[36 * 6] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    std::vector<const char*> materials;
};

#endif //PLAYGROUND_SIMPLELIGHTNINGSCENE_H

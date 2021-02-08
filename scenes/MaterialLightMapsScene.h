#ifndef PLAYGROUND_MATERIALLIGHTMAPSSCENE_H
#define PLAYGROUND_MATERIALLIGHTMAPSSCENE_H

#include "Scene.h"
#include "shapes.h"

class MaterialLightMapsScene : Scene {
public:
    MaterialLightMapsScene() {
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

        cubeShader = std::make_unique<Shader>("../shaders/material.vert", "../shaders/textured_box_point_light.frag");
        cubeShader->bind();

        texDiffuse = std::make_unique<Texture>("../res/textures/container/container.png", 0);
        texDiffuse->bind(0);
        texSpecular = std::make_unique<Texture>("../res/textures/container/container_specular.png", 1);
        texSpecular->bind(1);
        texEmission = std::make_unique<Texture>("../res/textures/black.png", 2);
        texEmission->bind(2);
        texShine = std::make_unique<Texture>("../res/textures/container/container_shine.png", 3);
        texShine->bind(3);
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
        Renderer::get().setClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.w);

        glm::mat4 transform { 1.0f };
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(1.0f, 0.3f, 0.5f));
        transform = glm::scale(transform, scale);
        cubeShader->setUniform3f("u_CameraPosition", camera->getPosition());
        cubeShader->setLight(light);
        Renderer::get().draw(*box.va, *box.ib, *cubeShader, transform);

        transform = glm::mat4 { 1.0f };
        transform = glm::translate(transform, light.position);
        transform = glm::scale(transform, glm::vec3(0.2f));
        lightSourceShader->setUniform3f("u_Color", light.diffuse);
        Renderer::get().draw(*lightSource.va, *lightSource.ib, *lightSourceShader, transform);
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

        ImGui::ColorEdit3("Light Source Ambient", &light.ambient.x);
        ImGui::ColorEdit3("Light Source Diffuse", &light.diffuse.x);
        ImGui::ColorEdit3("Light Source Specular", &light.specular.x);

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        ImGui::ColorEdit3("Background Color", &clearColor.r);

        ImGui::End();
    }

private:
    std::shared_ptr<Camera> camera = nullptr;

    std::unique_ptr<Shader> cubeShader, lightSourceShader;
    std::unique_ptr<Texture> texDiffuse, texSpecular, texEmission, texShine;

    PointLight light = {
            .position = glm::vec3 { 1.2f, 1.0f, 2.0f },
            .ambient = glm::vec3 { 0.2f, 0.2f, 0.2f },
            .diffuse = glm::vec3 { 0.5f, 0.5f, 0.5f },
            .specular = glm::vec3 { 1.0f, 1.0f, 1.0f },
            .constant = 1.0f,
            .linear = 0.09f,
            .quadratic = 0.032f
    };

    Material material = {
            .diffuse = 0,
            .specular = 1,
            .emission = 2,
            .shine = 3
    };

    float rotation = 0.0f;
    glm::vec3 scale = glm::vec3 { 1.0f };

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

    glm::vec4 clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
};

#endif //PLAYGROUND_MATERIALLIGHTMAPSSCENE_H
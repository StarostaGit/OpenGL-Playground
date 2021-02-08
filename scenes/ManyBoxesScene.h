#ifndef PLAYGROUND_MANYBOXESSCENE_H
#define PLAYGROUND_MANYBOXESSCENE_H

#include "Scene.h"

class ManyBoxesScene : public Scene {
public:
    ManyBoxesScene() {
        for (unsigned int i = 0; i < 36; i++) indices[i] = i;
    }

    std::shared_ptr<Camera> getCamera(float width, float height) override {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(width, height);

        camera->setDefaults(
                glm::vec3 { 0.0f, 0.0f, 3.0f },
                -90.0f,
                0.0f,
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
        layout->push(GL_FLOAT, 2);
        va->addBuffer(*vb, *layout);

        ib = std::make_unique<IndexBuffer>(indices, 36 );

        shader = std::make_unique<Shader>("../shaders/basic_texture.vert", "../shaders/two_textures.frag");
        shader->bind();

        texture1 = std::make_unique<Texture>("../res/textures/cpp_logo_alpha.png", 0);
        texture1->bind(0);
        shader->setUniform1i("u_Texture1", 0);
        texture2 = std::make_unique<Texture>("../res/textures/wood_material.jpeg", 1);
        texture2->bind(1);
        shader->setUniform1i("u_Texture2", 1);

        shader->unbind();
        vb->unbind();
        va->unbind();
        ib->unbind();

        Renderer::get().setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    }

    void draw() override {
        std::vector<glm::mat4> transforms;
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * i + rotation;
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            transform = glm::scale(transform, scale);
            transforms.push_back(transform);
        }

        Renderer::get().draw(*va, *ib, *shader, transforms);
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
        ImGui::End();
    }

private:
    std::unique_ptr<VertexArray> va;
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<IndexBuffer> ib;
    std::unique_ptr<VertexBufferLayout> layout;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture1, texture2;

    float rotation = 0.0f;
    glm::vec3 scale = glm::vec3 { 1.0f };

    unsigned int indices[36] = { 0 };

    float vertices[36 * 5] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
};

#endif //PLAYGROUND_MANYBOXESSCENE_H
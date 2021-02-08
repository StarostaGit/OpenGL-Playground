#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <cmath>
#include <memory>

#include "utils.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

#include "scenes/ManyBoxesScene.h"
#include "scenes/SimpleBoxScene.h"
#include "scenes/SimpleLightningScene.h"
#include "scenes/MaterialLightMapsScene.h"
#include "scenes/MultipleLightsScene.h"


struct ApplicationState {
    std::shared_ptr<Camera> camera;

    float lastFrame;
    float deltaTime;

    int width;
    int height;

    bool focusMode;
    float mouseX;
    float mouseY;

    void timeFrame() {
        auto current = (float) glfwGetTime();
        deltaTime = current - lastFrame;
        lastFrame = current;
    }
};

void processCameraMovement(GLFWwindow *window) {
    auto state = (ApplicationState*) glfwGetWindowUserPointer(window);
    auto &camera = state->camera;

    if (!camera)
        return;

    const auto &front = camera->getFront();
    const auto &right = camera->getRight();

    float speed = 2.5f * state->deltaTime;

    glm::vec3 movement { 0.0f };
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += speed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= speed * front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= right * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += right * speed;

    camera->moveCamera(movement.x, movement.y, movement.z);
}

void processControls(GLFWwindow *window) {
    auto state = (ApplicationState*) glfwGetWindowUserPointer(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        state->focusMode = !state->focusMode;
        if (state->focusMode)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 1720;
    int height = 1080;

    window = glfwCreateWindow(width, height, "Playground", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, utils::framebufferSizeCallback);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Couldn't initialize glew!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    utils::setupErrorHandling();

    ApplicationState state {
        .camera = std::make_shared<Camera>((float) width, (float) height),
        .lastFrame = 0.0f,
        .deltaTime = 0.0f,
        .width = width,
        .height = height,
        .focusMode = false,
        .mouseX = (float) width / 2,
        .mouseY = (float) height / 2,
    };

    glfwSetWindowUserPointer(window, (void*) &state);

    glfwSetWindowSizeCallback(window, [] (GLFWwindow *window, int width, int height) {
        auto state = (ApplicationState*) glfwGetWindowUserPointer(window);
        state->width = width; state->height = height;
    });
    glfwSetCursorPosCallback(window, [] (GLFWwindow *window, double xpos, double ypos) {
        const float sensitivity = 0.1f;

        auto state = (ApplicationState*) glfwGetWindowUserPointer(window);

        if (!state->focusMode) {
            state->mouseX = xpos;
            state->mouseY = ypos;
            return;
        }

        float xoffset = xpos - state->mouseX; xoffset *= sensitivity;
        float yoffset = ypos - state->mouseY; yoffset *= sensitivity;
        state->mouseX = xpos; state->mouseY = ypos;

        state->camera->rotateCamera(xoffset, -1.0f * yoffset);
    });
    glfwSetScrollCallback(window, [] (GLFWwindow *window, double xoffset, double yoffset) {
        const float zoomSpeed = 2.0f;

        auto state = (ApplicationState*) glfwGetWindowUserPointer(window);
        auto &camera = state->camera;

        if (!camera)
            return;

        float fov = camera->getFieldOfView();
        fov -= (float) yoffset * zoomSpeed;
        fov = std::min(fov, 120.0f); fov = std::max(fov, 1.0f);
        camera->setFieldOfView(fov);
    });

    {
//        MultipleLightsScene scene { window };
        MaterialLightMapsScene scene {};
        state.camera = scene.getCamera((float) width, (float) height);
        scene.setup();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460 core");

        while (!glfwWindowShouldClose(window)) {
            state.timeFrame();

            processControls(window);
            processCameraMovement(window);

            state.camera->setAspectRation((float) state.width, (float) state.height);
//            state.camera->printCurrentSetting();

            Renderer::get().setProjection(state.camera->getProjectionMatrix());
            Renderer::get().setView(state.camera->getViewMatrix());

            Renderer::get().clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            scene.draw();

            {
                ImGui::Begin("Camera Controls");
                if (ImGui::Button("Reset Zoom"))
                    state.camera->setFieldOfView(45.0f);
                if (ImGui::Button("Reset Camera"))
                    state.camera->reset();
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            scene.showSceneControlsWindow();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    };

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
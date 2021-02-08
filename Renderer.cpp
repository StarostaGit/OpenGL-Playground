#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer &Renderer::get() {
    static Renderer renderer;
    return renderer;
}

Renderer::Renderer()
    : projection(1.0f), view(1.0f) {}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setProjection(const glm::mat4 &projectionMatrix) {
    projection = projectionMatrix;
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader, const glm::mat4 &transform) const {
    shader.bind();
    va.bind();
    ib.bind();

    if (shader.uniformExists("u_Projection"))
        shader.setUniformMatrix4f("u_Projection", projection);
    if (shader.uniformExists("u_View"))
        shader.setUniformMatrix4f("u_View", view);
    if (shader.uniformExists("u_Model"))
        shader.setUniformMatrix4f("u_Model", transform);

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader,
                    const std::vector<glm::mat4> &transforms) const {
    shader.bind();
    va.bind();
    ib.bind();

    if (shader.uniformExists("u_Projection"))
        shader.setUniformMatrix4f("u_Projection", projection);
    if (shader.uniformExists("u_View"))
        shader.setUniformMatrix4f("u_View", view);

    for (const auto &transform : transforms) {
        if (shader.uniformExists("u_Model"))
            shader.setUniformMatrix4f("u_Model", transform);

        glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
    }
}

void Renderer::setClearColor(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
}

void Renderer::setView(const glm::mat4 &viewMatrix) {
    view = viewMatrix;
}

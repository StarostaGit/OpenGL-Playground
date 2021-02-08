#ifndef PLAYGROUND_RENDERER_H
#define PLAYGROUND_RENDERER_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Renderer {
public:
    static Renderer& get();

    Renderer();

    void draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader, const glm::mat4 &transform = glm::mat4(1.0f)) const;
    void draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader, const std::vector<glm::mat4> &transforms) const;
    void clear() const;

    void setClearColor(float r, float g, float b, float a) const;

    void setProjection(const glm::mat4 &projectionMatrix);
    void setView(const glm::mat4 &viewMatrix);

private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif //PLAYGROUND_RENDERER_H

#ifndef PLAYGROUND_VERTEXBUFFERLAYOUT_H
#define PLAYGROUND_VERTEXBUFFERLAYOUT_H

#include <GL/glew.h>
#include <vector>
#include <unordered_map>

class VertexBufferLayout {
public:
    struct LayoutElement {
        GLenum type;
        size_t count;
        size_t size;
        GLboolean normalized;
        size_t index;
    };

    inline void push(GLenum type, size_t count) {
        if (sizes.find(type) == sizes.end())
            throw std::runtime_error("Unsupported VA element type");

        elements.push_back({ type, count, sizes.at(type) * count, type == GL_UNSIGNED_BYTE, elements.size() });
        stride += elements.back().size;
    }

    [[nodiscard]] inline size_t getStride() const { return stride; };
    [[nodiscard]] inline const std::vector<LayoutElement>& getElements() const { return elements; }

private:
    size_t stride = 0;
    std::vector<LayoutElement> elements;

    static const std::unordered_map<GLenum, size_t> sizes;
};

#endif //PLAYGROUND_VERTEXBUFFERLAYOUT_H

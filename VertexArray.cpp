#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}

void VertexArray::addBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout) {
    buffer.bind();
    size_t offset = 0;
    for (const auto &element : layout.getElements()) {
        glEnableVertexAttribArray(element.index);
        glVertexAttribPointer(element.index, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
        offset += element.size;
    }
}

void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

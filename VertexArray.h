#ifndef PLAYGROUND_VERTEXARRAY_H
#define PLAYGROUND_VERTEXARRAY_H

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout);

    void bind() const;
    void unbind() const;

private:
    GLuint id;
};


#endif //PLAYGROUND_VERTEXARRAY_H

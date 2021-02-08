#ifndef PLAYGROUND_VERTEXBUFFER_H
#define PLAYGROUND_VERTEXBUFFER_H

#include <GL/glew.h>

class VertexBuffer {
public:
    VertexBuffer(const void *data, size_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    GLuint id;
};


#endif //PLAYGROUND_VERTEXBUFFER_H

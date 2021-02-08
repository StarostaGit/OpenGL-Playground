#ifndef PLAYGROUND_INDEXBUFFER_H
#define PLAYGROUND_INDEXBUFFER_H


#include <GL/glew.h>

class IndexBuffer {
public:
    IndexBuffer(const uint32_t *data, size_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    [[nodiscard]] inline size_t getCount() const { return count; };

private:
    GLuint id;
    size_t count;
};


#endif //PLAYGROUND_INDEXBUFFER_H

#ifndef PLAYGROUND_TEXTURE_H
#define PLAYGROUND_TEXTURE_H

#include "GL/glew.h"
#include <iostream>

class Texture {
public:
    explicit Texture(const std::string &path, int slot = 0);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

private:
    GLuint id;
    const std::string path;
    const uint8_t *buffer;
    int width;
    int height;
    int bpp;
};


#endif //PLAYGROUND_TEXTURE_H

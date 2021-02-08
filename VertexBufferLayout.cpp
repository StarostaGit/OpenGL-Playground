#include "VertexBufferLayout.h"

const std::unordered_map<GLenum, size_t> VertexBufferLayout::sizes {
        { GL_FLOAT, sizeof(GLfloat) },
        { GL_UNSIGNED_BYTE, sizeof(GLubyte) },
        { GL_UNSIGNED_INT, sizeof(GLuint) }
};
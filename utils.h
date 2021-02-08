#ifndef PLAYGROUND_UTILS_H
#define PLAYGROUND_UTILS_H

namespace utils {

    void setupErrorHandling() {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageCallback([] (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *param) {
            std::cout << "[OpenGL Error] " << std::string(message, length) << std::endl;
        }, nullptr);
    }

    void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

}

#endif //PLAYGROUND_UTILS_H

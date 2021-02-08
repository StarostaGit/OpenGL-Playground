#ifndef PLAYGROUND_SHADER_H
#define PLAYGROUND_SHADER_H

#include "Material.h"
#include "Light.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

class Shader {
public:
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    bool uniformExists(const std::string &name);

    void setUniform1i(const std::string &name, GLint v0);
    void setUniform1f(const std::string &name, GLfloat v0);
    void setUniform2f(const std::string &name, GLfloat v0, GLfloat v1);
    void setUniform3f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2);
    void setUniform3f(const std::string &name, const glm::vec3 &v);
    void setUniform4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void setUniformMatrix4f(const std::string &name, const glm::mat4 &matrix);
    void setMaterial(const SimpleMaterial &material);
    void setMaterial(const std::string &name, const SimpleMaterial &material);
    void setMaterial(const Material &material);
    void setMaterial(const std::string &name, const Material &material);
    void setLight(const DirectionalLight &light);
    void setLight(const std::string &name, const DirectionalLight &light);
    void setLight(const PointLight &light);
    void setLight(const std::string &name, const PointLight &light);
    void setLight(const Spotlight &light);
    void setLight(const std::string &name, const Spotlight &light);

private:
    GLuint id;
    std::unordered_map<std::string, GLint> cache;

    static std::string loadShaderFromFile(const std::string &filepath);
    static GLuint createShader(const std::string &vertexShader, const std::string &fragmentShader);
    static GLuint compileShader(GLenum type, const std::string &source);

    GLint getUniformLocation(const std::string &name);
};


#endif //PLAYGROUND_SHADER_H

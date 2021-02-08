#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    auto vertexShader = loadShaderFromFile(vertexShaderPath);
    auto fragmentShader = loadShaderFromFile(fragmentShaderPath);

    id = createShader(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::bind() const {
    glUseProgram(id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

std::string Shader::loadShaderFromFile(const std::string &filepath) {
    std::ifstream fs { filepath };
    std::string source;

    fs.seekg(0, std::ios::end);
    source.reserve(fs.tellg());
    fs.seekg(0, std::ios::beg);

    source.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    return std::move(source);
}

GLuint Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

GLuint Shader::compileShader(GLenum type, const std::string &source) {
    GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char message [length];
        glGetShaderInfoLog(shader, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLint Shader::getUniformLocation(const std::string &name) {
    if (auto it = cache.find(name); it != cache.end())
        return it->second;

    GLint loc = glGetUniformLocation(id, name.c_str());
    if (loc == -1)
        std::cout << "[Warning] Uniform '" << name << "' not found in shader" << std::endl;

    cache[name] = loc;
    return loc;
}

void Shader::setUniform1f(const std::string &name, GLfloat v0) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, v0);
}

void Shader::setUniform2f(const std::string &name, GLfloat v0, GLfloat v1) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v0, v1);
}

void Shader::setUniform3f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v0, v1, v2);
}

void Shader::setUniform3f(const std::string &name, const glm::vec3 &v) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::setUniform4f(const std::string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v0, v1, v2, v3);
}

void Shader::setUniform1i(const std::string &name, GLint v0) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, v0);
}

void Shader::setUniformMatrix4f(const std::string &name, const glm::mat4 &matrix) {
    bind();
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMaterial(const SimpleMaterial &material) {
    setMaterial("u_Material", material);
}

void Shader::setMaterial(const std::string &name, const SimpleMaterial &material) {
    setUniform3f(name + ".ambient", material.ambient);
    setUniform3f(name + ".diffuse", material.diffuse);
    setUniform3f(name + ".specular", material.specular);
    setUniform1f(name + ".shine", material.shine);
}

void Shader::setMaterial(const Material &material) {
    setMaterial("u_Material", material);
}

void Shader::setMaterial(const std::string &name, const Material &material) {
    setUniform1i(name + ".diffuse", material.diffuse);
    setUniform1i(name + ".specular", material.specular);
    setUniform1i(name + ".emission", material.emission);
    setUniform1i(name + ".shine", material.shine);
}

void Shader::setLight(const DirectionalLight &light) {
    setLight("u_Light", light);
}

void Shader::setLight(const std::string &name, const DirectionalLight &light) {
    setUniform3f(name + ".direction", light.direction);
    setUniform3f(name + ".ambient", light.ambient);
    setUniform3f(name + ".diffuse", light.diffuse);
    setUniform3f(name + ".specular", light.specular);
}

void Shader::setLight(const PointLight &light) {
    setLight("u_Light", light);
}

void Shader::setLight(const std::string &name, const PointLight &light) {
    setUniform3f(name + ".position", light.position);
    setUniform3f(name + ".ambient", light.ambient);
    setUniform3f(name + ".diffuse", light.diffuse);
    setUniform3f(name + ".specular", light.specular);
    setUniform1f(name + ".constant", light.constant);
    setUniform1f(name + ".linear", light.linear);
    setUniform1f(name + ".quadratic", light.quadratic);
}

void Shader::setLight(const Spotlight &light) {
    setLight("u_Light", light);
}

void Shader::setLight(const std::string &name, const Spotlight &light) {
    setUniform3f(name + ".position", light.position);
    setUniform3f(name + ".direction", light.direction);
    setUniform3f(name + ".ambient", light.ambient);
    setUniform3f(name + ".diffuse", light.diffuse);
    setUniform3f(name + ".specular", light.specular);
    setUniform1f(name + ".innerCutoff", light.innerCutoff);
    setUniform1f(name + ".outerCutoff", light.outerCutoff);
    setUniform1f(name + ".constant", light.constant);
    setUniform1f(name + ".linear", light.linear);
    setUniform1f(name + ".quadratic", light.quadratic);
}

bool Shader::uniformExists(const std::string &name) {
    return getUniformLocation(name) != -1;
}

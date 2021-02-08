#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(float width, float height)
    : position(defaultPosition),
    front(0.0f, 0.0f, -1.0f),
    right(1.0f, 0.0f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
    yaw(defaultYaw), pitch(defaultPitch),
    fov(defaultFov), width(width), height(height) {
    updateVectors();
}

void Camera::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void Camera::setRotation(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
}

void Camera::setAspectRation(float width, float height) {
    this->width = width;
    this->height = height;
}

void Camera::setFieldOfView(float angle) {
    fov = angle;
}

void Camera::zoom(float delta) {
    fov += delta;
}

void Camera::moveCamera(float x, float y, float z) {
    position += glm::vec3 { x, y, z };
}

const glm::vec3& Camera::getFront() const {
    return front;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

const glm::vec3& Camera::getUp() const {
    return up;
}

const glm::vec3& Camera::getRight() const {
    return right;
}

float Camera::getFieldOfView() const {
    return fov;
}

void Camera::rotateCamera(float yaw, float pitch) {
    this->pitch += pitch;
    this->yaw += yaw;
    this->pitch = std::min(this->pitch, 89.0f);
    this->pitch = std::max(this->pitch, -89.0f);
    updateVectors();
}

void Camera::updateVectors() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::setDefaults(const glm::vec3 &pos, float yaw, float pitch, float fov) {
    defaultPosition = pos;
    defaultFov = fov;
    defaultPitch = pitch;
    defaultYaw = yaw;
}

void Camera::reset() {
    position = defaultPosition;
    fov = defaultFov;
    yaw = defaultYaw;
    pitch = defaultPitch;
    updateVectors();
}

void Camera::printCurrentSetting() const {
    std::cout << "Camera {\n";
    std::cout << "\tPos: { " << position.x << " " << position.y << " " << position.z << " }\n";
    std::cout << "\tYaw: " << yaw << "\n";
    std::cout << "\tPitch: " << pitch << "\n";
    std::cout << "\tFov: " << fov << "\n";
    std::cout << "}" << std::endl;
}

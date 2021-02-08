#ifndef PLAYGROUND_CAMERA_H
#define PLAYGROUND_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float width, float height);

    [[nodiscard]] const glm::vec3& getFront() const;
    [[nodiscard]] const glm::vec3& getPosition() const;
    [[nodiscard]] const glm::vec3& getUp() const;
    [[nodiscard]] const glm::vec3& getRight() const;
    [[nodiscard]] float getFieldOfView() const;

    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const;

    void setDefaults(const glm::vec3 &pos, float yaw, float pitch, float fov);

    void setPosition(float x, float y, float z);
    void setRotation(float yaw, float pitch);
    void setAspectRation(float width, float height);
    void setFieldOfView(float angle);

    void zoom(float delta);
    void moveCamera(float x, float y, float z);
    void rotateCamera(float yaw, float pitch);
    void reset();

    void printCurrentSetting() const;

private:
    glm::vec3 defaultPosition = glm::vec3 { 0.0f };
    float defaultYaw = 0.0f;
    float defaultPitch = 0.0f;
    float defaultFov = 45.0f;
    glm::vec3 worldUp = glm::vec3 { 0.0f, 1.0f, 0.0f };

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float yaw;
    float pitch;
    float fov;
    float width;
    float height;

    void updateVectors();
};


#endif //PLAYGROUND_CAMERA_H

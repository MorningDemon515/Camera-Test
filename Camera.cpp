#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, CameraMode mode)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch), mode(mode),
      movementSpeed(5.0f), mouseSensitivity(0.1f) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(SDL_Scancode key, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (key == SDL_SCANCODE_W) position += front * velocity;
    if (key == SDL_SCANCODE_S) position -= front * velocity;
    if (key == SDL_SCANCODE_A) position -= right * velocity;
    if (key == SDL_SCANCODE_D) position += right * velocity;

    if (mode == FLIGHT) {
        if (key == SDL_SCANCODE_Q) position -= up * velocity;
        if (key == SDL_SCANCODE_E) position += up * velocity;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (mode == FPS) pitch = glm::clamp(pitch, -89.0f, 89.0f); // FPS 模式限制俯仰
    }

    updateCameraVectors();
}

void Camera::switchMode(CameraMode newMode) {
    mode = newMode;
    if (mode == FPS) pitch = glm::clamp(pitch, -89.0f, 89.0f); // 切换到 FPS 时重新限制俯仰角
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
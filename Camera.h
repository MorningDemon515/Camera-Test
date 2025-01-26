#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <SDL.h>

enum CameraMode {
    FLIGHT, // 飞行模式
    FPS     // FPS 模式
};

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, CameraMode mode);

    glm::mat4 getViewMatrix() const;
    void processKeyboard(SDL_Scancode key, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void switchMode(CameraMode newMode);

private:
    void updateCameraVectors();

    CameraMode mode;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
};

#endif
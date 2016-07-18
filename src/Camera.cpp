#include "Camera.h"
#include <SDL2/SDL.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 windowDims, float fov, float nearPlane, float farPlane, float vel, float angularVel, glm::vec3 pos, float xRot, float yRot)
    : projMatrix_(glm::perspective(fov, windowDims.x / windowDims.y, nearPlane, farPlane)), 
      vel_(vel), angular_(angularVel), pos_(pos), orientation_(xRot, yRot, 0.f)
{
    glm::mat4 viewRotX = glm::rotate(glm::mat4(1.f), -orientation_.x, glm::vec3(0, 1, 0));
    glm::mat4 viewRot = glm::rotate(viewRotX, -orientation_.y, glm::vec3(1, 0, 0));
    viewMatrix_ = glm::translate(viewRot, -pos);
    viewProjMatrix_ = projMatrix_ * viewMatrix_;
}

glm::mat4x4 const & Camera::getViewMatrix() const {
    return viewMatrix_;
}

glm::mat4x4 const & Camera::getProjectionMatrix() const {
    return projMatrix_;
}

glm::mat4x4 const & Camera::getViewProjectionMatrix() const
{
    return viewProjMatrix_;
}

void Camera::update(float deltaTime)
{
    int mouseDeltaX, mouseDeltaY;
    SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
    
    orientation_.x -= mouseDeltaX * angular_;
    orientation_.y -= mouseDeltaY * angular_;

    // Update rotation and look direction
    glm::mat4 rotMat(glm::eulerAngleYXZ(-orientation_.x, -orientation_.y, 0.f));

    glm::mat4 dirRot(glm::eulerAngleYXZ(orientation_.x, orientation_.y, 0.f));
    glm::vec3 lookDir(dirRot * glm::vec4(0, 0, -1, 1));
    glm::vec3 sideDir(dirRot * glm::vec4(1, 0, 0, 1));

    // Update position
    const Uint8* kbd = SDL_GetKeyboardState(nullptr);
    float x = 0;
    float z = 0;
    if (kbd[SDL_SCANCODE_W]) {
        z = vel_ * deltaTime;
    }
    if (kbd[SDL_SCANCODE_S]) {
        z = -vel_ * deltaTime;
    }
    if (kbd[SDL_SCANCODE_A]) {
        x = -vel_ * deltaTime;
    }
    if (kbd[SDL_SCANCODE_D]) {
        x = vel_ * deltaTime;
    }

    glm::vec3 lookTrans(lookDir * z);
    glm::vec3 sideTrans(sideDir * x);
    pos_ += lookTrans + sideTrans;

    viewMatrix_ = glm::lookAt(pos_, pos_ + lookDir, glm::vec3(0, 1, 0));
    viewProjMatrix_ = projMatrix_ * viewMatrix_;
}

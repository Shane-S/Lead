#include "Camera.h"
#include <SDL2/SDL.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 windowDims, float fov, float nearPlane, float farPlane, float vel, float angularVel, glm::vec3 pos, float xRot, float yRot)
    : projMatrix_(glm::perspective(fov, windowDims.x / windowDims.y, nearPlane, farPlane)), 
      vel_(vel), angular_(angularVel), pos_(pos), orientation_(xRot, yRot, 0.f)
{
    glm::mat4x4 rot(glm::rotate(orientation_.x, glm::vec3(0, 1, 0)) * glm::rotate(orientation_.y, glm::vec3(1, 0, 0)));
    viewMatrix_ = glm::translate(-pos) * rot;
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

    orientation_.x += mouseDeltaX * angular_;
    orientation_.y += mouseDeltaY * angular_;

    // Update rotation and look direction
    glm::mat4x4 rotMat(glm::rotate(orientation_.x, glm::vec3(0, 1, 0)) * glm::rotate(orientation_.y, glm::vec3(1, 0, 0)));
    glm::vec3 lookDir(rotMat * glm::vec4( 0, 0, -1.f, 0 ));
    glm::vec3 sideDir(rotMat * glm::vec4( 1.f, 0, 0, 0 ));

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
    pos_ += (lookDir * z) + (sideDir * x);

    viewMatrix_ = glm::translate(-pos_) * rotMat;
    viewProjMatrix_ = projMatrix_ * viewMatrix_;
}

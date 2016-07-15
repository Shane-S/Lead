#pragma once

#include <glm/glm.hpp>

class Camera {

public:
    Camera(glm::vec2 windowDims, float fov, float nearPlane, float farPlane, float vel, float angularVel,
           glm::vec3 pos = { 0, 0, 0 }, float xRot = 0.f, float yRot = 0.f);

    glm::mat4x4 const& getViewProjectionMatrix() const;
    glm::mat4x4 const& getProjectionMatrix() const;
    glm::mat4x4 const& getViewMatrix() const;
    /**
     * Updates orientation and position and the view matrix.
     * @param deltaTime The number of seconds since the last update.
     */
    void update(float deltaTime);

private:
    glm::vec3 pos_;
    glm::vec3 orientation_;
    float vel_;
    float angular_;

    glm::mat4x4 viewMatrix_;
    glm::mat4x4 projMatrix_;
    glm::mat4x4 viewProjMatrix_;
};
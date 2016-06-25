#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera {

public:
    Camera(float fov, )
    glm::mat4x4 const& getViewMatrix() const;
    glm::mat4x4 const& getProjectionMatrix() const;

private:
    glm::vec3   pos_;
    glm::mat4x4 viewMatrix_;
    glm::mat4x4 projMatrix_;
    glm::vec3 rotation_;
    float nearPlane_;
    float farPlane_;
};
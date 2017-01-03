#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Scene;

class Camera {

public:
    Camera(glm::vec2 windowDims, float fov, float nearPlane, float farPlane, float vel, float angularVel, std::string reticle,
           glm::vec3 pos = { 0, 0, 0 }, float xRot = 0.f, float yRot = 0.f);

    glm::mat4x4 const& getViewProjectionMatrix() const;
    glm::mat4x4 const& getProjectionMatrix() const;
    glm::mat4x4 const& getViewMatrix() const;
    
    /**
     * Updates orientation/position and the view matrix.
     * @param deltaTime The number of seconds since the last update.
     */
    void update(float deltaTime);

    /**
     * Draws the reticle indicating the centre of the camera.
     */
    void draw(Scene const& scene);

private:
    glm::vec3 pos_;
    glm::vec3 orientation_;
    float vel_;
    float angular_;

    glm::mat4 viewMatrix_;
    glm::mat4 projMatrix_;
    glm::mat4 viewProjMatrix_;

    GLuint reticleVAO_;
    GLuint reticleVBO_;
    GLuint reticleTex_;
    glm::mat4 reticleProj_;
};
#define GLM_FORCE_RADIANS
#include "Camera.h"
#include "Scene.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLProgramUtils.h"

// Vertices for the reticle to draw
//  x     y     u     v
static float rverts[] = {
   -5.f, -5.f,  0.f,  1.f,
    5.f,  5.f,  1.f,  0.f,
   -5.f,  5.f,  0.f,  0.f,
   -5.f, -5.f,  0.f,  1.f,
    5.f, -5.f,  1.f,  1.f,
    5.f,  5.f,  1.f,  0.f
};

VertexAttribute reticleSpec[] = {
    { VertexAttribPos, 2, GL_FLOAT, GL_FALSE, 16, 0 },
    { VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 16, 8 },
};

Camera::Camera(glm::vec2 windowDims, float fov, float nearPlane, float farPlane, float vel, float angularVel, std::string reticle,
               glm::vec3 pos, float xRot, float yRot)
    : projMatrix_(glm::perspective(glm::radians(fov), windowDims.x / windowDims.y, nearPlane, farPlane)),
      reticleProj_(glm::translate(glm::ortho(0.f, windowDims.x, 0.f, windowDims.y), { windowDims.x / 2, windowDims.y / 2, 0.f })),
      vel_(vel), angular_(angularVel), pos_(pos), orientation_(glm::radians(xRot), glm::radians(yRot), 0.f)
{
    viewMatrix_ = glm::translate(glm::yawPitchRoll(-orientation_.x, -orientation_.y, 0.f), -pos);
    viewProjMatrix_ = projMatrix_ * viewMatrix_;

    // Set up the reticle
    if (!setupTexture(reticle.c_str(), &reticleTex_)) {
        throw "Couldn't set up reticle texture!";
    }

    glGenVertexArrays(1, &reticleVAO_);
    glBindVertexArray(reticleVAO_);

    glGenBuffers(1, &reticleVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, reticleVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rverts), rverts, GL_STATIC_DRAW);
    
    setVertexAttributes(reticleSpec, 2);
    glBindVertexArray(0);
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
    const Uint8* kbd = SDL_GetKeyboardState(nullptr);
    if (kbd[SDL_SCANCODE_R]) {
        pos_ = { 0, 0, 0 };
        orientation_ = { 0, 0, 0 };
        return;
    }
 
    int mouseDeltaX, mouseDeltaY;
    SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
    
    orientation_.x -= mouseDeltaX * angular_;
    orientation_.y -= mouseDeltaY * angular_;
    
    orientation_.y = glm::clamp(orientation_.y, glm::radians(-89.9f), glm::radians(89.9f));

    // Update rotation and look direction
    glm::mat4 rotMat(glm::yawPitchRoll(-orientation_.x, -orientation_.y, 0.f));

    glm::mat4 dirRot(glm::yawPitchRoll(orientation_.x, orientation_.y, 0.f));
    glm::vec3 lookDir(dirRot * glm::vec4(0, 0, -1, 1));
    glm::vec3 sideDir(dirRot * glm::vec4(1, 0, 0, 1));

    // Update position
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

void Camera::draw(Scene const& scene)
{
    const Shader& orthoShader = scene.getShaders().find("ortho")->second;
    orthoShader.makeCurrent();

    glBindVertexArray(reticleVAO_);
    glBindTexture(GL_TEXTURE_2D, reticleTex_);

    glm::mat4 ident(1.f);

    glUniformMatrix4fv(orthoShader.getUniforms().find("mvp")->second, 1, GL_FALSE, glm::value_ptr(reticleProj_));

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"
#include "Utils.h"
#include "TestModelVerts.h"
#include <iostream>

#define CAMERA_TOGGLE SDLK_c

Scene::Scene(SDL_Window* window)
    : models_(), shaders_(), window_(window),
    cam_({ SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h }, 45.f, 0.5f, 400.f, 8.f, 0.005f),
    camFocused_(false)
{
    
	test = 50;
	tweakBar_ = TwNewBar("libpb Params");
	TwAddVarRW(tweakBar_, "Width", TW_TYPE_INT32, &test, " label='Wnd width' help='Width of the graphics window (in pixels)' ");

    ShaderAttribute modelShaderAttrs[] = {
        {VertexAttribPos, "position"},
        {VertexAttribNormal, "normal"},
        {VertexAttribTexCoord0, "texCoordIn"}
    };

    char* modelShaderUniforms[] = {
        "mvp"
    };

    shaders_.emplace("model", Shader(std::vector<ShaderAttribute>(&modelShaderAttrs[0], &modelShaderAttrs[0] + 3),
                               std::vector<std::string>(&modelShaderUniforms[0], &modelShaderUniforms[0] + 1),
                               "assets/shader.vert", "assets/shader.frag"));

    std::vector<VertexAttribute> attrs(shipVertSpec, shipVertSpec + numShipAttrs);
    models_.push_back(Model{ shipTexture, attrs, shipVerts, numShipVerts });
    models_[0].pos = glm::vec3(0.f, 0.f, -3.f);
    models_[0].scale = glm::vec3(0.07f, 0.07f, 0.07f);
}

Scene::~Scene() {
	TwDeleteBar(tweakBar_);
}

void Scene::twUpdate(SDL_Event& ev) {
    switch (ev.type) {
    case SDL_KEYUP:
    {
        int translated = SDLToTwKey(ev.key.keysym.sym);
        if (translated == -1) break;

        TwKeyPressed(translated, ev.key.keysym.mod);
    }
    break;
    case SDL_MOUSEMOTION:
        TwMouseMotion(ev.motion.x, ev.motion.y);
        break;
    case SDL_MOUSEBUTTONUP:
        TwMouseButton(TW_MOUSE_RELEASED, (TwMouseButtonID)ev.button.button);
        break;
    case SDL_MOUSEBUTTONDOWN:
        TwMouseButton(TW_MOUSE_PRESSED, (TwMouseButtonID)ev.button.button);
        break;
    case SDL_MOUSEWHEEL:
        TwMouseWheel(ev.wheel.y);
        break;
    }
}

void Scene::focusCam() {
    int fake;
    SDL_SetRelativeMouseMode((SDL_bool)1);
    SDL_GetRelativeMouseState(&fake, &fake); // Flush the relative mouse state so that we don't report something stupid
    SDL_ShowCursor(0);
}

void Scene::focusTw() {
    SDL_Surface* wndSurface = SDL_GetWindowSurface(window_);

    SDL_SetRelativeMouseMode((SDL_bool)0);
    SDL_WarpMouseInWindow(window_, wndSurface->w / 2, wndSurface->h / 2);
    SDL_ShowCursor(1);
}

bool Scene::update(float deltaTime) {
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_KEYUP:
            if (ev.key.keysym.sym == CAMERA_TOGGLE) {
                camFocused_ = !camFocused_;

                if (camFocused_) {
                    focusCam();
                }
                else {
                    focusTw();
                    twUpdate(ev);
                }
            } else if (!camFocused_) {
                twUpdate(ev);
            }
            break;
        case SDL_QUIT:
            return false;
        default:
            if (!camFocused_) {
                twUpdate(ev);
            }
            break;
        }
    }

    if (camFocused_) {
        cam_.update(deltaTime);
    }

    return true;
}

void Scene::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	TwDraw();

    for (auto& model : models_) {
        model.draw(*this);
    }
}

Camera const & Scene::getCamera() const
{
    return cam_;
}

std::map<std::string, Shader> const & Scene::getShaders() const
{
    return shaders_;
}

#include <SDL2/SDL.h>
#include <iostream>
#include "Scene.h"
#include "Utils.h"
#include "TestModelVerts.h"

#define CAMERA_TOGGLE SDLK_c

Scene::Scene(SDL_Window* window)
    : drawList_(), shaders_(), window_(window),
    cam_({ SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h }, 45.f, 0.5f, 400.f, 8.f, 0.005f, "assets/reticle.png"),
    camFocused_(false)
{
    
	test = 50;
	tweakBar_ = TwNewBar("libpb Params");
	TwAddVarRW(tweakBar_, "Width", TW_TYPE_INT32, &test, " label='Wnd width' help='Width of the graphics window (in pixels)' ");



    // Create the shader used for models
    std::vector<ShaderAttribute> modelShaderAttrs = {
        {VertexAttribPos, "position"},
        {VertexAttribNormal, "normal"},
        {VertexAttribTexCoord0, "texCoordIn"}
    };
    std::vector<std::string> modelShaderUniforms = {
        "mvp"
    };
    shaders_.emplace("model", Shader(modelShaderAttrs, modelShaderUniforms, "assets/shader.vert", "assets/shader.frag"));

    // Create the shader used for 2D projections (the floor plan)
    std::vector<ShaderAttribute> orthoShaderAttrs = {
        {VertexAttribPos, "position"},
        {VertexAttribTexCoord0, "texCoordIn"}
    };
    std::vector<std::string> orthoShaderUniforms = {
        "mvp"
    };
    shaders_.emplace("ortho", Shader(orthoShaderAttrs, orthoShaderUniforms, "assets/ortho_shader.vert", "assets/ortho_shader.frag"));

    std::vector<VertexAttribute> attrs(cubeVertSpec, cubeVertSpec + numCubeAttrs);
    drawList_.push_back(std::shared_ptr<Drawable>(new Drawable3D{ cubeTexture, attrs, cubeVerts, numCubeVerts, true }));
    auto ptr = std::static_pointer_cast<Drawable3D>(drawList_[0]);
    ptr->pos = glm::vec3(0.f, 0.f, -3.f);
    ptr->scale = glm::vec3(1.f, 1.f, 1.f);
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

    for (auto& drawable : drawList_) {
        drawable->draw(*this);
    }

    if (camFocused_) {
        cam_.draw(*this);
    }

    TwDraw();
}

Camera const & Scene::getCamera() const
{
    return cam_;
}

std::map<std::string, Shader> const & Scene::getShaders() const
{
    return shaders_;
}

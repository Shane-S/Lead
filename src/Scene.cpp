#include <SDL2/SDL.h>
#include <iostream>
#include <pb/util/hashmap/hash_utils.h>
#include <pb/sq_house.h>
#include "Scene.h"
#include "Utils.h"
#include "TestModelVerts.h"
#include "FloorPlan.h"

#define CAMERA_TOGGLE SDLK_c
#define GENERATE_HOUSE_CAMERA_MODE SDLK_g

void TW_CALL generateHouseCallback(void* scene)
{
    Scene* scn = reinterpret_cast<Scene*>(scene);
    scn->generateHouse();
}

void TW_CALL reseedRand(void* scene)
{
    Scene* scn = reinterpret_cast<Scene*>(scene);
    scn->reseedRandom();
}

void TW_CALL randomDimsSet(void const* value, void* scene)
{
    bool* val = (bool*)value;
    Scene* scn = reinterpret_cast<Scene*>(scene);
    scn->setDimsAreRandom(*val);
}

void TW_CALL randomDimsGet(void* value, void* scene)
{
    Scene* scn = reinterpret_cast<Scene*>(scene);
    bool* val = (bool*)value;
    *val = scn->areDimsRandom();
}

static char const* living_adj[] = {
        PB_SQ_HOUSE_OUTSIDE,
        PB_SQ_HOUSE_STAIRS,
        "Laundry room",
        "Kitchen",
        "Dining room",
        "Bathroom",
        "Bedroom",
        "Master bedroom",
};

static char const* kitchen_adj[] = {
        PB_SQ_HOUSE_OUTSIDE,
        "Pantry",
        "Laundry room",
        "Living room",
        "Dining room"
};

static char const* pantry_adj[] = {
        "Laundry room",
        "Kitchen"
};

static char const* laundry_adj[] = {
        PB_SQ_HOUSE_STAIRS,
        "Kitchen",
        "Pantry",
};

static char const* dining_adj[] = {
        "Kitchen",
        "Living room",
        "Bathroom",
        "Bedroom",
        "Master bedroom",
};

static char const* bathroom_adj[] = {
        PB_SQ_HOUSE_STAIRS,
        "Living room",
        "Dining room",
        "Bedroom",
        "Master bedroom",
};

static char const* bedroom_adj[] = {
        PB_SQ_HOUSE_STAIRS,
        "Living room",
        "Dining room",
        "Bathroom",
};

static char const* master_adj[] = {
        PB_SQ_HOUSE_STAIRS,
        "Living room",
        "Dining room",
        "Bathroom",
};

static pb_sq_house_room_spec specs[8];

Scene::Scene(SDL_Window* window)
    : drawList_(), shaders_(), window_(window),
    cam_({ SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h }, 45.f, 0.5f, 400.f, 8.f, 0.005f, "assets/reticle.png"),
    textures_(),
    camFocused_(false), plan_(nullptr), building_(nullptr),
    seed_(0xDEADBEEF), randomDimensions_(false)
{
    //--------------------------Initialise Room Specifications----------------------------//
    specs[0].name = "Living room";
    specs[0].adjacent = &living_adj[0];
    specs[0].num_adjacent = sizeof(living_adj) / sizeof(char*);
    specs[0].priority = 0;
    specs[0].max_instances = 1;
    specs[0].area = 20.f;

    specs[1].name = "Kitchen";
    specs[1].adjacent = &kitchen_adj[0];
    specs[1].num_adjacent = sizeof(kitchen_adj) / sizeof(char*);
    specs[1].priority = 1;
    specs[1].max_instances = 1;
    specs[1].area = 15.f;

    specs[2].name = "Pantry";
    specs[2].adjacent = &pantry_adj[0];
    specs[2].num_adjacent = sizeof(pantry_adj) / sizeof(char*);
    specs[2].priority = 6;
    specs[2].max_instances = 1;
    specs[2].area = 5.f;

    specs[3].name = "Laundry room";
    specs[3].adjacent = &laundry_adj[0];
    specs[3].num_adjacent = sizeof(laundry_adj) / sizeof(char*);
    specs[3].priority = 4;
    specs[3].max_instances = 1;
    specs[3].area = 9.f;

    specs[4].name = "Dining room";
    specs[4].adjacent = &dining_adj[0];
    specs[4].num_adjacent = sizeof(dining_adj) / sizeof(char*);
    specs[4].priority = 5;
    specs[4].max_instances = 1;
    specs[4].area = 15.f;

    specs[5].name = "Bathroom";
    specs[5].adjacent = &bathroom_adj[0];
    specs[5].num_adjacent = sizeof(bathroom_adj) / sizeof(char*);
    specs[5].priority = 2;
    specs[5].max_instances = 5;
    specs[5].area = 7.f;

    specs[6].name = "Bedroom";
    specs[6].adjacent = &bedroom_adj[0];
    specs[6].num_adjacent = sizeof(bedroom_adj) / sizeof(char*);
    specs[6].priority = 3;
    specs[6].max_instances = 5;
    specs[6].area = 10.f;

    specs[7].name = "Master bedroom";
    specs[7].adjacent = &master_adj[0];
    specs[7].num_adjacent = sizeof(master_adj) / sizeof(char*);
    specs[7].priority = 7;
    specs[7].max_instances = 1;
    specs[7].area = 15.f;

    size_t i;
    roomSpecs_ = pb_hashmap_create(pb_str_hash, pb_str_eq);
    for (i = 0; i < 8; ++i) {
        pb_hashmap_put(roomSpecs_, specs[i].name, &specs[i]);
    }

    houseSpec_.num_rooms = 15;
    houseSpec_.door_size = 0.75f;
    houseSpec_.window_size = 0.5f;
    houseSpec_.hallway_width = 0.75f;
    houseSpec_.stair_room_width = 3.f;
    houseSpec_.width = 15.f;
    houseSpec_.height = 10.f;

    //--------------------------Tweak Bar Init----------------------------//
	tweakBar_ = TwNewBar("libpb");
	TwAddVarRW(tweakBar_, "random_seed", TW_TYPE_UINT32, &seed_, " label='Random Seed' help='Seed value for the rand() function.' ");
    TwAddButton(tweakBar_, "reseed_btn", reseedRand, &seed_, " label='Re-seed rand()' help='Re-seed the rand() function with the current seed.' ");

    TwAddVarRW(tweakBar_, "num_rooms", TW_TYPE_UINT32, &houseSpec_.num_rooms,
        " label='Number of Rooms' help='Number of rooms to include in the generated house. ");
    TwDefine(" libpb/num_rooms min=1 max=15 ");

    TwAddVarRW(tweakBar_, "house_width", TW_TYPE_FLOAT, &houseSpec_.width,
        " label='House Width' help='The generated house's width. ");
    TwDefine(" libpb/house_width min=9 ");

    TwAddVarRW(tweakBar_, "house_depth", TW_TYPE_FLOAT, &houseSpec_.height,
        " label='House Depth' help='The generated house's depth (distance along the z-axis). ");
    TwDefine(" libpb/house_depth min=9");

    TwAddVarCB(tweakBar_, "random_dims", TW_TYPE_BOOLCPP, randomDimsSet, randomDimsGet, this,
        " label='Toggle Random Dimensions' help='Decides whether to generate random dimensions or use the ones entered by the user.' ");

    TwAddButton(tweakBar_, "Generate", generateHouseCallback, this, " label='Generate House' help='Generates a new floor plan and house.' ");

    //--------------------------Create test model----------------------------//

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

    //--------------------------Load textures----------------------------//
    textures_.emplace("Bathroom floor", 0);
    textures_.emplace("Laundry room floor", 0);
    textures_.emplace("Bedroom floor", 0);
    textures_.emplace("Master bedroom floor", 0);
    textures_.emplace("Living room floor", 0);
    textures_.emplace("Kitchen floor", 0);
    textures_.emplace("Pantry floor", 0);
    textures_.emplace("Dining room floor", 0);
    textures_.emplace("Hardwood tile floor", 0);
    textures_.emplace("Stucco wall", 0);
    textures_.emplace("Stucco ceiling", 0);
    textures_.emplace("Wood siding", 0);

    for (auto& entry : textures_) {
        std::string path = "assets/" + entry.first + ".jpg";

        if (!setupTexture(path.c_str(), &entry.second)) {
            throw "Couldn't load texture " + path;
        }
    }
}

Scene::~Scene()
{
	TwDeleteBar(tweakBar_);
    for (auto& entry : textures_) {
        glDeleteTextures(1, &entry.second);
    }
}

void Scene::twUpdate(SDL_Event& ev)
{
    int err;
    switch (ev.type)
    {
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

void Scene::focusCam()
{
    int fake;
    SDL_SetRelativeMouseMode((SDL_bool)1);
    SDL_GetRelativeMouseState(&fake, &fake); // Flush the relative mouse state so that we don't report something stupid
    SDL_ShowCursor(0);
}

void Scene::focusTw()
{
    SDL_Surface* wndSurface = SDL_GetWindowSurface(window_);

    SDL_SetRelativeMouseMode((SDL_bool)0);
    SDL_WarpMouseInWindow(window_, wndSurface->w / 2, wndSurface->h / 2);
    SDL_ShowCursor(1);
}

bool Scene::update(float deltaTime)
{
    SDL_Event ev;

    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_KEYUP:
            if (ev.key.keysym.sym == CAMERA_TOGGLE)
            {
                camFocused_ = !camFocused_;

                if (camFocused_)
                {
                    focusCam();
                }
                else
                {
                    focusTw();
                    twUpdate(ev);
                }
            }
            else if (ev.key.keysym.sym == GENERATE_HOUSE_CAMERA_MODE && camFocused_)
            {
                generateHouse();
            }
            else if (!camFocused_)
            {
                twUpdate(ev);
            }
            break;
        case SDL_QUIT:
            return false;
        default:
            if (!camFocused_)
            {
                twUpdate(ev);
            }
            break;
        }
    }
 
    if (camFocused_)
    {
        cam_.update(deltaTime);
    }

    for (auto& drawable : drawList_)
    {
        drawable->update(deltaTime);
    }

    return true;
}

void Scene::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto& drawable : drawList_)
    {
        drawable->draw(*this);
    }

    if (camFocused_)
    {
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

void Scene::generateHouse()
{
    if (plan_)
    {
        drawList_.pop_back();
    }

    if (randomDimensions_)
    {
        unsigned int n1 = rand() % 43;
        unsigned int n2 = rand() % 43;

        houseSpec_.width = 9.f + (float)n1 / 2.f;
        houseSpec_.height = 9.f + (float)n2 / 2.f;
    }

    plan_ = std::shared_ptr<FloorPlan>(new FloorPlan(roomSpecs_, &houseSpec_));
    building_ = std::shared_ptr<Building>(new Building(*plan_, textures_));

    drawList_.push_back(building_);

    // Stop showing the test asset
    drawList_[0]->setVisible(false);
}

void Scene::setDimsAreRandom(bool dimsAreRandom)
{
    randomDimensions_ = dimsAreRandom;
    if (randomDimensions_)
    {
        TwDefine(" libpb/house_width readonly=true ");
        TwDefine(" libpb/house_depth readonly=true ");
    }
    else
    {
        TwDefine(" libpb/house_width readonly=false ");
        TwDefine(" libpb/house_depth readonly=false ");
    }
}

bool Scene::areDimsRandom() const
{
    return randomDimensions_;
}

void Scene::reseedRandom() const
{
    srand(seed_);
}

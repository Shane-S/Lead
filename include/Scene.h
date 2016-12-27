#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <AntTweakBar.h>
#include <SDL2/SDL.h>
#include <pb/util/hashmap/hashmap.h>
#include <pb/sq_house.h>
#include "Drawable3D.h"
#include "Shader.h"
#include "FloorPlan.h"
#include "Building.h"

/**
 * A simple scene with a camera, models to draw, and a GUI.
 */
class Scene {
public:
	// Don't need copy constructor etc. since I'm only going to have one Scene
	Scene(SDL_Window* window);
	~Scene();

	/**
	 * Updates all elements in the scene.
	 * @param deltaTime The time, in seconds, since the last update.
	 * @return Whether to continue running (return false will break out of the run loop).
	 */
	bool update(float deltaTime);

	/**
	 * Draws all drawable elements in the scene.
	 */
	void draw();

    Camera const& getCamera() const;
    std::map<std::string, Shader> const& getShaders() const;

    void generateHouse();

private:
    // Update function for AntTweakBar
    void twUpdate(SDL_Event& ev);

    // Function to set focus on the camera
    void focusCam();

    // Function to set focus on the Tweak Bar
    void focusTw();

    Camera cam_;
    bool camFocused_;

	std::vector<std::shared_ptr<Drawable>> drawList_;
	std::map<std::string, Shader> shaders_;
    std::map<std::string, GLuint> textures_; // Textures for libpb

	TwBar* tweakBar_;
    SDL_Window* window_;

    // libpb stuff
    std::shared_ptr<FloorPlan> plan_;
	std::shared_ptr<Building> building_;

    unsigned int seed_;
	pb_hashmap* roomSpecs_;
    pb_sq_house_house_spec houseSpec_;
};
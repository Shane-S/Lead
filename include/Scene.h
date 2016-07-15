#pragma once
#include <vector>
#include <map>
#include <string>
#include <AntTweakBar.h>
#include <SDL2/SDL.h>
#include "Model.h"
#include "Shader.h"

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

private:
    // Update function for AntTweakBar
    void twUpdate(SDL_Event& ev);

    // Function to set focus on the camera
    void focusCam();

    // Function to set focus on the Tweak Bar
    void focusTw();

    Camera cam_;
    bool camFocused_;

	std::vector<Model> models_;
	std::map<std::string, Shader> shaders_;
	int test;

	TwBar* tweakBar_;
    SDL_Window* window_;
    
};
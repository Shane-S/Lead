#pragma once
#include <vector>
#include <map>
#include <string>
#include <AntTweakBar.h>
#include "Model.h"
#include "Shader.h"

/**
 * A simple scene with a camera, models to draw, and a GUI.
 */
class Scene {
public:
	// Don't need copy constructor etc. since I'm only going to have one Scene
	Scene();
	~Scene();

	/**
	 * Updates all elements in the scene.
	 * @param deltaTime The time, in seconds, since the last update.
	 * @return Whether to continue running (return false will break out of the run loop).
	 */
	bool Update(double deltaTime);

	/**
	 * Draws all drawable elements in the scene.
	 */
	void Draw();

private:
	std::vector<Model> models_;
	std::map<std::string, Shader> shaders_;
	int test;
	TwBar* tweakBar_;
};
#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"

Scene::Scene() 
	: models_(), shaders_()
{
	tweakBar_ = TwNewBar("libpb Params");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with SDL and OpenGL.\nPress [Space] to toggle fullscreen.' ");
}

Scene::~Scene() {
	TwDeleteBar(tweakBar_);
}

bool Scene::Update(double deltaTime) {
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			return false;
		}
	}
	return true;
}

void Scene::Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
}

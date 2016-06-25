#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"
#include "Utils.h"

Scene::Scene() 
	: models_(), shaders_()
{
	test = 50;
	tweakBar_ = TwNewBar("libpb Params");
	TwAddVarRW(tweakBar_, "Width", TW_TYPE_INT32, &test, " label='Wnd width' help='Width of the graphics window (in pixels)' ");
}

Scene::~Scene() {
	TwDeleteBar(tweakBar_);
}

bool Scene::Update(double deltaTime) {
	SDL_Event ev;

	// Let AntTweakBar do some stuff
	while (SDL_PollEvent(&ev)) {
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
		case SDL_QUIT:
			return false;
		}
	}
	return true;
}

void Scene::Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	TwDraw();
}

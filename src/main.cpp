#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <AntTweakBar.h>
#include <iostream>
#include <GL/glew.h>
#include "Scene.h"

int main(int argc, char **argv) {

	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Error while intialising SDL: " << SDL_GetError() << std::endl;
		std::cin.get();
		return 1;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) < 0) {
		std::cerr << "Error while initialising SDL_image: " << IMG_GetError() << std::endl;
		std::cin.get();
		return 2;
	}

	SDL_Window* window = SDL_CreateWindow("Lead", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Initialise AntTweakBar
	if (!TwInit(TW_OPENGL, nullptr) || !TwWindowSize(1280, 720)) {
		std::cerr << "Error while initialising AntTweakbar: " << TwGetLastError() << std::endl;
		std::cin.get();
		return 3;
	}

	// Initialise GLEW
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "Error while initialising GLEW: " << glewGetErrorString(result) << std::endl;
		std::cin.get();
		return 4;
	}

	glEnable(GL_CULL_FACE);
	glClearColor(103.f / 255, 193.f / 255, 244.f / 255, 1.f);

	Scene scene;
	Uint32 lastTicks = SDL_GetTicks();
	Uint32 currentTicks;
	
	while (true) {
		// Limit the frame rate to ~60fps
		currentTicks = SDL_GetTicks();
		Uint32 delta = currentTicks - lastTicks;
		if (delta < 16) continue;

		double deltaSecs = delta = 1000.0;
		if (!scene.Update(deltaSecs)) break; // OK, this is a bit ugly, but it works
		scene.Draw();

		SDL_GL_SwapWindow(window);

		// Give the CPU a break
		SDL_Delay(1);

		lastTicks = currentTicks;
	}

	TwTerminate();
	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(context);
	SDL_Quit();
    return 0;
}
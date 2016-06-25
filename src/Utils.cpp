#include <map>
#include <SDL2/SDL.h>
#include <AntTweakBar.h>
#include "Utils.h"

std::map<int, int> sdl2tw =
{
	{ SDLK_BACKSPACE, TW_KEY_BACKSPACE },
	{ SDLK_TAB, TW_KEY_TAB },
	{ SDLK_CLEAR, TW_KEY_CLEAR },
	{ SDLK_RETURN, TW_KEY_RETURN },
	{ SDLK_PAUSE, TW_KEY_PAUSE },
	{ SDLK_ESCAPE, TW_KEY_ESCAPE },
	{ SDLK_SPACE, TW_KEY_SPACE },
	{ SDLK_DELETE, TW_KEY_DELETE },
	{ SDLK_UP, TW_KEY_UP },
	{ SDLK_DOWN, TW_KEY_DOWN },
	{ SDLK_RIGHT, TW_KEY_RIGHT },
	{ SDLK_LEFT, TW_KEY_LEFT },
	{ SDLK_INSERT, TW_KEY_INSERT },
	{ SDLK_HOME, TW_KEY_HOME },
	{ SDLK_END, TW_KEY_END },
	{ SDLK_PAGEUP, TW_KEY_PAGE_UP },
	{ SDLK_PAGEDOWN, TW_KEY_PAGE_DOWN },
	{ SDLK_F1, TW_KEY_F1 },
	{ SDLK_F2, TW_KEY_F2 },
	{ SDLK_F3, TW_KEY_F3 },
	{ SDLK_F4, TW_KEY_F4 },
	{ SDLK_F5, TW_KEY_F5 },
	{ SDLK_F6, TW_KEY_F6 },
	{ SDLK_F7, TW_KEY_F7 },
	{ SDLK_F8, TW_KEY_F8 },
	{ SDLK_F9, TW_KEY_F9 },
	{ SDLK_F10, TW_KEY_F10 },
	{ SDLK_F11, TW_KEY_F11 },
	{ SDLK_F12, TW_KEY_F12 },
	{ SDLK_F13, TW_KEY_F13 },
	{ SDLK_F14, TW_KEY_F14 },
	{ SDLK_F15, TW_KEY_F15 }
};

int SDLToTwKey(int sdlKey) {
	// Printable ASCII characters are represented by their actual character value in SDL and AntTweakBar
	if (sdlKey > 32 && sdlKey < 127) return sdlKey;

	auto val = sdl2tw.find(sdlKey);
	if (val == sdl2tw.end()) return -1;
	return val->second;
}
#pragma once

#include <SDL.h>
#include <stdio.h>

/*
* Singleton implementation of a graphic window using the SDL2 library from https://www.libsdl.org/.
*/
class Graphic {
private:
	static Graphic* instance;
	SDL_Window* window;
	SDL_Surface* screenSurface;
	Graphic();
	~Graphic();
public:
	static Graphic* getInstance();
};
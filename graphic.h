#pragma once

#include <SDL.h>
#include <stdio.h>

/*
* Singleton implementation of graphic window.
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
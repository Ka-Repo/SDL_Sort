#pragma once

#include <SDL.h>
#include <stdio.h>

class Graphic {
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;

public:
	Graphic();
	~Graphic();
};
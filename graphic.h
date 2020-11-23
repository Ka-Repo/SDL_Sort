#pragma once

#include <SDL.h>
#include <stdio.h>

class graphic {
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;

public:
	graphic();
	~graphic();
};
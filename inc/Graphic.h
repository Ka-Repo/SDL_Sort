#pragma once

#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <string>

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
	void displayData(std::vector<int> data, std::string text, SDL_Color textColor);
	void loadText(std::string text, SDL_Color textColor);
	void update();
};
#pragma once

#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <iostream>

/*
* Singleton implementation of a graphic window using the SDL2 library from https://www.libsdl.org/.
*/
class Graphic 
{
private:
	static Graphic* instance;
	SDL_Window* window;
	SDL_Surface* screenSurface;
	Graphic();
	~Graphic();
public:
	static Graphic* getInstance();
	template<typename T> void displayData(std::vector<T> data, std::string text, SDL_Color textColor);
	void loadText(std::string text, SDL_Color textColor);
	void update();
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Graphic* Graphic::instance = NULL;
TTF_Font* font = NULL;

Graphic::Graphic()
{
	window = NULL;
	screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}
	else
	{
		window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
		}
		else
		{
			if (TTF_Init() == -1)
			{
				std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
			}

			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
		}

	}
}

Graphic::~Graphic()
{
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	font = NULL;
	instance = NULL;
}

Graphic* Graphic::getInstance()
{
	if (!instance)
	{
		instance = new Graphic();
	}

	return instance;
}

template<typename T>
void Graphic::displayData(std::vector<T> data, std::string text, SDL_Color textColor)
{
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		SDL_Rect rect = { i, SCREEN_HEIGHT - data[i], 1, data[i] };

		SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0xFF));
	}

	loadText(text, textColor);
	update();
}

void Graphic::loadText(std::string text, SDL_Color textColor)
{
	font = TTF_OpenFont("fonts/IndieFlower.ttf", 20);

	SDL_Texture* texture = NULL;

	if (font == NULL)
	{
		std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << "\n";
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);

	if (textSurface == NULL)
	{
		std::cout << "Unable to create texture from rendered text: " << SDL_GetError() << "\n";
	}
	else
	{
		SDL_Rect textRect = { SCREEN_WIDTH / 2 - textSurface->w / 2, 10, textSurface->w,  textSurface->h };

		SDL_BlitSurface(textSurface, NULL, screenSurface, &textRect);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(texture);
	}
}

void Graphic::update()
{
	SDL_UpdateWindowSurface(window);
}
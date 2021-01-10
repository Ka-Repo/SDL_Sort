#include "Graphic.h"
#include <SDL_ttf.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Graphic* Graphic::instance = NULL;

Graphic::Graphic()
{
	window = NULL;
	screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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
	// TTF_CloseFont(font);
	// font = NULL;
	TTF_Quit();
	SDL_Quit();

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

void Graphic::displayData(std::vector<int> data, std::string text, SDL_Color textColor)
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
	TTF_Font* font = TTF_OpenFont("fonts/IndieFlower.ttf", 20);

	SDL_Texture* texture = NULL;

	if (font == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);

	if (textSurface == NULL) {
		printf("Unable to create texture from rendered text: %s\n", SDL_GetError());
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

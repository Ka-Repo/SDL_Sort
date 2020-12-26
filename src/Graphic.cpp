#include "Graphic.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Graphic* Graphic::instance = nullptr;

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
		window = SDL_CreateWindow("Sorting with SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
			SDL_UpdateWindowSurface(window);
		}
	}
}

Graphic::~Graphic()
{
	SDL_DestroyWindow(window);
	SDL_Quit();

	instance = nullptr;
}

Graphic* Graphic::getInstance()
{
	if (!instance)
	{
		instance = new Graphic();
	}

	return instance;
}

void Graphic::displayData(std::vector<int> data)
{
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		SDL_Rect rect = { i, 0, 5, data[i] };
		SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	}

	SDL_UpdateWindowSurface(window);
}
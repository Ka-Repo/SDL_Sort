#include "Graphic.h"

#include <random>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	Graphic* screen = Graphic::getInstance();
    SDL_Event event;
    bool quit = false;

	std::uniform_int_distribution<int> distribution{ 0, SCREEN_HEIGHT };
	std::mt19937 generator{ std::random_device{}() };
	std::vector<int> data(SCREEN_WIDTH);

	auto generate = [&distribution, &generator]() {
		return distribution(generator);
	};

	std::generate(data.begin(), data.end(), generate);
    screen->displayData(data);

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    // Quicksort
                    break;
                case SDLK_s:
                    // Standard Sort
                    break;
                case SDLK_m:
                    // Merge Sort
                    break;
                case SDLK_r:
                    // Reset
                    screen->displayData(data);
                    break;
                case SDLK_g:
                    // Generate new data
                    std::generate(data.begin(), data.end(), generate);
                    screen->displayData(data);
                    break;
                default:
                    break;
                }
            }
        }
    }

	return 0;
}
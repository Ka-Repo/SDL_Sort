#include "Graphic.h"

#include <random>
#include <iostream>
#include <sstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	Graphic* screen = Graphic::getInstance();
    SDL_Event event;
    bool quit = false;
    std::stringstream time;

	std::uniform_int_distribution<int> distribution{ 0, SCREEN_HEIGHT };
	std::mt19937 generator{ std::random_device{}() };
	std::vector<int> data(SCREEN_WIDTH);

	auto generate = [&distribution, &generator]() {
		return distribution(generator);
	};

	std::generate(data.begin(), data.end(), generate);
 
    time.str("");
    time << "Time elapsed : " << "      " << "Operations used :";

    screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });

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
                    screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });
                    break;
                case SDLK_s:
                    // Standard Sort
                    std::sort(data.begin(), data.end(), std::less<int>());
                    screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });
                    break;
                case SDLK_m:
					// Merge Sort
                    screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });
                    break;
                case SDLK_r:
                    // Reset
                    screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });
                    break;
                case SDLK_g:
                    // Generate new data
                    std::generate(data.begin(), data.end(), generate);
                    screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });
                    break;
                default:
                    break;
                }
            }
        }
    }

	return 0;
}
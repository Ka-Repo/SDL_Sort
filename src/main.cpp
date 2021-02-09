#include "Graphic.h"
#include "Algorithms.h"

#include <random>
#include <iostream>
#include <sstream>
#include <chrono>

int main(int argc, char* args[])
{
	Graphic* screen = Graphic::getInstance();
	SDL_Event event;
	std::stringstream time;
	bool quit = false;

	using clock_t = std::chrono::steady_clock;
	std::chrono::microseconds timeElapsed = std::chrono::microseconds(0);

	std::uniform_int_distribution<int> distribution{ 0, SCREEN_HEIGHT };
	std::mt19937 generator{ std::random_device{}() };
	std::vector<int> data(SCREEN_WIDTH);
	std::vector<int> initialData;

	auto generate = [&distribution, &generator]() {
		return distribution(generator);
	};

	std::generate(data.begin(), data.end(), generate);
	initialData = data;

	time << "Time elapsed : " << timeElapsed.count() << "us";

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
				clock_t::time_point start = clock_t::now();
				clock_t::time_point end;

				switch (event.key.keysym.sym)
				{
				case SDLK_q:
					// Quicksort
					Algorithms::quicksort(data.begin(), data.end(), std::greater<int>());
					break;
				case SDLK_s:
					// Standard Sort
					std::sort(data.begin(), data.end(), std::greater<int>());
					break;
				case SDLK_m:
					// Merge Sort
					Algorithms::mergesort(data.begin(), data.end(), std::greater<int>());
					break;
				case SDLK_z:
					// Selection Sort
					Algorithms::selectionsort(data.begin(), data.end(), std::less<int>());
					break;
				case SDLK_b:
					// Bubble Sort
					Algorithms::bubblesort(data.begin(), data.end(), std::less<int>());
					break;
				case SDLK_r:
					// Reset
					data = initialData;
					break;
				case SDLK_g:
					// Generate new data
					std::generate(data.begin(), data.end(), generate);
					initialData = data;
					break;
				case SDLK_4:
				case SDLK_2:
					// Bogo sort
					// Algorithms::bogosort(data.begin(), data.end(), std::less<int>());
					break;
				default:
					break;
				}

				end = clock_t::now();

				std::vector<SDL_Keycode> codes { SDLK_q, SDLK_s, SDLK_m, SDLK_z, SDLK_b, SDLK_4, SDLK_2 };

				(std::find(codes.begin(), codes.end(), event.key.keysym.sym) != codes.end())
					? timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
					: timeElapsed = std::chrono::microseconds(0);

				time.str("");
				time << "Time elapsed : " << timeElapsed.count() << "us";
				screen->displayData(data, time.str().c_str(), { 0, 0, 0, 0 });
			}
		}
	}

	return 0;
}
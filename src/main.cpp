#include "Graphic.h"

#include <random>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	Graphic* screen = Graphic::getInstance();

	std::uniform_int_distribution<int> distribution{ 0, SCREEN_HEIGHT };
	std::mt19937 generator{ std::random_device{}() };
	std::vector<int> data(SCREEN_WIDTH);

	auto generate = [&distribution, &generator]() {
		return distribution(generator);
	};

	std::generate(data.begin(), data.end(), generate);

	for (auto i : data) std::cout << i << "\n";

	while (1);
	return 0;
}
#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>


class SDL_Lock {
	inline static std::size_t instances = 0;

public:
	SDL_Lock() {
		if(!instances) {
			if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
				std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
				exit(1);
			}
			int imgFlags = IMG_INIT_PNG | IMG_INIT_PNG | IMG_INIT_TIF;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				std::cerr << "IMG_Init: " << IMG_GetError() << '\n';
				exit(2);
			}
			if(TTF_Init() < 0) {
				std::cerr << "TTF_Init: " << TTF_GetError() << '\n';
				exit(3);
			}
		}
		++instances;
	}
	
	SDL_Lock(SDL_Lock const&) {
		++instances;
	}
	
	SDL_Lock& operator=(SDL_Lock const&) {
		return *this;
	}
	
	SDL_Lock(SDL_Lock&&) {
		++instances;
	}
	
	SDL_Lock& operator=(SDL_Lock&&) {
		return *this;
	}
	
	~SDL_Lock() {
		--instances;
		if(!instances) {
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
		}
	}
};

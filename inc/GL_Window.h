#pragma once

#include <string>
#include "SDL_Lock.h"

class GL_Window {
private:
	SDL_Lock      _lock;
	SDL_Window*   _window;
	SDL_GLContext _gl_context;
	uint32_t      _window_id;

private:
	GL_Window(GL_Window const&) = delete;
	GL_Window& operator=(GL_Window const&) = delete;

public:
	template<typename OP, typename... As>
	auto do_operation(OP op, As&&... as) const {
		SDL_GL_MakeCurrent(_window, _gl_context);
		return op(std::forward<As>(as)...);
	}
	
	template<typename OP, typename... As>
	void draw(OP op, As&&... as)  const {
		do_operation(
			[&op,this](As&&... as) {
				glClearDepth(20.0);
				glClearColor(0.13, 0.12, 0.13, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				op(std::forward<As>(as)...);
				SDL_GL_SwapWindow(_window);
			}
			, std::forward<As>(as)...
		);
	}
	
	void set_title(std::string const& title) {
		SDL_SetWindowTitle(_window, title.c_str());
	}
	
	uint32_t window_id() const {
		return _window_id;
	}
	
	void dimensions(int& width, int& height) const {
		SDL_GetWindowSize(_window, &width, &height);
	}

	~GL_Window() {
		if(_window) {
			SDL_GL_DeleteContext(_gl_context);
			SDL_DestroyWindow(_window);
		}
	}
	
	GL_Window(GL_Window&& other)
		: _window(other._window)
		, _gl_context(std::move(other._gl_context))
	{
		other._window = nullptr;
	}

	GL_Window(std::string const& title, int width, int height)
		: _window(
			SDL_CreateWindow(
				  title.c_str()
				, SDL_WINDOWPOS_UNDEFINED
				, SDL_WINDOWPOS_UNDEFINED
				, width
				, height
				,     SDL_WINDOW_OPENGL
					| SDL_WINDOW_SHOWN
					| SDL_WINDOW_RESIZABLE
			)
		)
	{
		if(!_window) {
			std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
			exit(4);
		}
		_window_id = SDL_GetWindowID(_window);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		_gl_context = SDL_GL_CreateContext(_window);
		if(!_gl_context) {
			std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << '\n';
			exit(5);
		}
		
		SDL_GL_MakeCurrent(_window, _gl_context);
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if(glewError != GLEW_OK) {
			std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewError) << '\n';
			exit(6);
		}
		if(SDL_GL_SetSwapInterval(1) < 0) {
			std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << '\n';
		}
	}
};

template<typename DrawOp>
void scoped_draw(DrawOp op) {
	glPushMatrix();
	op();
	glPopMatrix();
}

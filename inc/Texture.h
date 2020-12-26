#pragma once
#include <cassert>
#include "GL_Window.h"

class Texture {
private:
	GL_Window const* _parent;
	unsigned int     _texture;
	int              _width;
	int              _height;

private:
	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;

public:
	Texture(Texture&& other)
		: _parent( other._parent )
		, _texture(other._texture)
		, _width(  other._width  )
		, _height( other._height )
	{
		other._texture = 0;
	}
	
	Texture& operator=(Texture&& other) {
		if(&other != this) {
			assert(other._parent == _parent && "Texture moved to wrong GL_Context");
			this->~Texture();
			_texture = other._texture;
			_width   = other._width;
			_height  = other._height;
			other._texture = 0;
		}
		return *this;
	}

	Texture(GL_Window const* parent)
		: _parent(parent)
		, _width(0)
		, _height(0)
	{
		parent->do_operation(
			[&](){
				glGenTextures(1, &_texture);
				bind();
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		);
	}

	Texture(GL_Window const* parent, int width, int height, unsigned char* data)
		: Texture(parent)
	{
		load(width, height, data);
	}

	Texture(GL_Window const* parent, SDL_Surface const* surface)
		: Texture(
			parent, surface->w, surface->h, (unsigned char*)surface->pixels
		)
	{}
	
	Texture(GL_Window const* parent, std::string const& filename)
		: Texture(parent)
	{
		SDL_Surface* surface_1 = IMG_Load(filename.c_str());
		if(surface_1 == nullptr) {
			std::cerr << "Unable to load image "<< filename << "! SDL_image Error: " << IMG_GetError() << '\n';
		}
		std::cerr << "loaded Texture: " << filename << " (" << surface_1->w << "x" << surface_1->h << ")\n";
		SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
		SDL_Surface* surface_2 = SDL_ConvertSurface(surface_1, format, 0 );
		if(surface_2 == nullptr) {
			std::cerr << "Unable to convert image " << filename << "! SDL Error: " << SDL_GetError() << '\n';
		}
		SDL_FreeFormat(format);
		SDL_FreeSurface(surface_1);
		load(surface_2->w, surface_2->h, (unsigned char*)surface_2->pixels);
		SDL_FreeSurface(surface_2);
	}

	~Texture() {
		if(_texture) {
			_parent->do_operation(
				[&](){
					glDeleteTextures(1, &_texture);
				}
			);
		}
	}

	void load(int width, int height, unsigned char* data) {
		if(_texture) {
			_parent->do_operation(
				[&](){
					bind();
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}
			);
			this->_width  = width;
			this->_height = height;
		}
	}
	
	void bind() const {
		if(_texture) {
			glBindTexture(GL_TEXTURE_2D, _texture);
		}
	}
	
	int width() const {
		return _width;
	}
	int height() const {
		return _height;
	}
};

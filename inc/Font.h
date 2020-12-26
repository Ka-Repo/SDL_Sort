#pragma once

#include <vector>
#include <array>
#include <memory>

#include "GL_Window.h"
#include "Texture.h"
#include "Mesh.h"

class Font {
private:
	struct Vertex {
		std::array<float,3> position;
		std::array<float,3> normal;
		std::array<float,2> texture;
	};
	
	struct Glyph {
		Texture const texture;
		Mesh<Vertex> mesh;
		float width;
		float height;

		Glyph(GL_Window const* parent, SDL_Surface* surface, double text_height)
			: texture(parent, surface)
			, mesh(parent)
			, width(surface->w * text_height / surface->h)
			, height(text_height)
		{
			std::vector<uint32_t> indices{0,1,2,0,2,3};
			std::vector<Vertex> vertices(4);
			auto& A = vertices[0];
			auto& B = vertices[1];
			auto& C = vertices[2];
			auto& D = vertices[3];
			A.position = {0,    -height, 0};
			B.position = {width,-height, 0};
			C.position = {width, 0,      0};
			D.position = {0,     0,      0};
			A.normal = {0,0,1};
			B.normal = A.normal;
			C.normal = A.normal;
			D.normal = A.normal;
			A.texture = {0,1};
			B.texture = {1,1};
			C.texture = {1,0};
			D.texture = {0,0};
			mesh.load<GL_STATIC_DRAW>(vertices, indices);
		}
	};

private:
	double text_height;
	std::vector<std::unique_ptr<Glyph>> glyphs;
	
public:
	Font(GL_Window const* parent, std::string const& name, int size, double text_height)
		: text_height(text_height)
	{
		std::shared_ptr<TTF_Font> font(
			  TTF_OpenFont(name.c_str(), size)
			, TTF_CloseFont
		);
		if(!font) {
			std::cerr << "Error: could not load font: " << name << '\n';
			exit(7);
		}
		glyphs.resize(256);
		for(int i = 1; i < 256; ++i) {
			SDL_Color color{255, 255, 255, 255};
			std::shared_ptr<SDL_Surface> surface(
				  TTF_RenderGlyph_Blended(font.get(), i, color)
				, SDL_FreeSurface
			);
			if(!surface) {
				std::cerr << "Warning: could not render glyph: " << name << " (char: " << i << ")\n";
			} else {
				glyphs[i] = std::make_unique<Glyph>(parent, surface.get(), text_height);
			}
		}
	}

	double text_width(std::string const& text) const {
		double w = 0.0;
		for(auto& c : text) {
			auto& g = glyphs[c];
			if(g) {
				w += g->width;
			}
		}
		return w;
	}

	double render(
		  std::string const& text
		, double x
		, double y
		, double z
	) const {
		double x0 = x;
		glEnable(GL_TEXTURE_2D);
		for(auto& c : text) {
			auto& g = glyphs[c];
			if(g) {
				scoped_draw([&]() {
					glTranslated(x,y,z);
					g->texture.bind();
					g->mesh.lock().draw();
				});
				x += g->width;
			}
		}
		glDisable(GL_TEXTURE_2D);
		return x;
	}
};

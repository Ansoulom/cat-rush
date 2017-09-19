#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include "SDL_deleter.h"

namespace Game
{
	namespace Graphics
	{
		class Texture
		{
		public:
			Texture(SDL_Renderer& renderer, std::string file_path);
			Texture(SDL_Renderer& renderer, std::string text, SDL_Color text_color, TTF_Font& font);
			Texture(std::unique_ptr<SDL_Surface, SDL_deleter> surface, SDL_Renderer& renderer);
			~Texture();

			void set_color(Uint8 red, Uint8 green, Uint8 blue);
			void set_alpha(Uint8 alpha);
			void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
			int get_width() const;
			int get_height() const;

		private:
			SDL_Renderer* renderer;
			std::unique_ptr<SDL_Texture, SDL_deleter> texture;
			int width;
			int height;

			friend class Renderer;
		};
	}
}
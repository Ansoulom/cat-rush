#pragma once

#include <SDL.h>
#include <memory>
#include "SDL_deleter.h"
#include <filesystem>


namespace Game
{
	namespace Text
	{
		class Font;
	}
}


namespace Game
{
	namespace Graphics
	{
		class Color;
		class Renderer;


		class Texture
		{
		public:
			Texture(Renderer& renderer, const std::filesystem::path& file_path);
			Texture(Renderer& renderer, const std::string& text, Color text_color, const Text::Font& font);
			Texture(const Texture& other) = delete;
			Texture(Texture&& other) noexcept;
			
			Texture& operator=(const Texture& other) = delete;
			Texture& operator=(Texture&& other) noexcept;

			int width() const;
			int height() const;

		private:
			Texture(std::unique_ptr<SDL_Surface, Sdl_deleter> surface, Renderer& renderer);

			std::unique_ptr<SDL_Texture, Sdl_deleter> texture_;
			int width_;
			int height_;

			friend class Renderer;
		};
	}
}

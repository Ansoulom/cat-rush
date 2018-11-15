#pragma once
#include <memory>
#include <SDL_ttf.h>
#include "SDL_deleter.h"
#include <filesystem>


namespace Game
{
	namespace Wrappers
	{
		class Sdl_wrapper;
	}


	namespace Text
	{
		class Font
		{
		public:
			Font(const std::filesystem::path& file_path, int size);

		private:
			std::unique_ptr<TTF_Font, Sdl_deleter> sdl_font_;

			friend class Wrappers::Sdl_wrapper;
		};
	}
}

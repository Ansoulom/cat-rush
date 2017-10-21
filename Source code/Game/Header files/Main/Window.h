#pragma once

#include <string>
#include <memory>
#include <SDL.h>
#include "SDL_deleter.h"
#include "Vector.h"


namespace Game
{
	namespace Graphics
	{
		class Renderer;
	}


	namespace Core
	{
		enum class Window_fullscreen_mode
		{
			fullscreen, borderless, windowed
		};

		class Window
		{
		public:
			Window(std::string name, int x, int y, int width, int height, Window_fullscreen_mode fullscreen_mode, bool visible);

			void set_title(const std::string& title);
			void set_visible(bool visible);
			void set_position(Geometry::Vector<int> position);
			Geometry::Vector<int> get_position() const;
			void set_size(Geometry::Vector<int> size);
			Geometry::Vector<int> get_size() const;

		private:
			std::unique_ptr<SDL_Window, Sdl_deleter> sdl_window_;

			friend class Graphics::Renderer;
		};
	}
}

#pragma once

#include <SDL.h>
#include <Shapes.h>


namespace Game
{
	class SDL_wrapper
	{
	public:
		SDL_wrapper();
		~SDL_wrapper();

		static SDL_Rect get_rect(const Geometry::Rectangle<int>& rect);
		static SDL_Point get_point(const Geometry::Vector<int>& point);

	private:
		static int count;
	};
}

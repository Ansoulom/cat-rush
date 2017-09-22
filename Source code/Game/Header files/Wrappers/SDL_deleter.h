#pragma once

#include <SDL.h>
#include <SDL_ttf.h>


namespace Game
{
	class Sdl_deleter
	{
	public:
		void operator()(SDL_Window* window) const;
		void operator()(SDL_Renderer* renderer) const;
		void operator()(SDL_Texture* texture) const;
		void operator()(TTF_Font* font) const;
		void operator()(char* text) const;
		void operator()(SDL_Surface* surface) const;
		void operator()(SDL_GameController* controller) const;
	};
}

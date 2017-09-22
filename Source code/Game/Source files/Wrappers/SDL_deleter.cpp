#include "SDL_deleter.h"

namespace Game
{
	void Sdl_deleter::operator()(SDL_Window* window) const
	{
		SDL_DestroyWindow(window);
	}

	void Sdl_deleter::operator()(SDL_Renderer* renderer) const
	{
		SDL_DestroyRenderer(renderer);
	}

	void Sdl_deleter::operator()(SDL_Texture* texture) const
	{
		SDL_DestroyTexture(texture);
	}

	void Sdl_deleter::operator()(TTF_Font* font) const
	{
		TTF_CloseFont(font);
	}

	void Sdl_deleter::operator()(char* text) const
	{
		SDL_free(text);
	}

	void Sdl_deleter::operator()(SDL_Surface* surface) const
	{
		SDL_FreeSurface(surface);
	}

	void Sdl_deleter::operator()(SDL_GameController* controller) const
	{
		SDL_GameControllerClose(controller);
	}
}

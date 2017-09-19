#include "SDL_deleter.h"

namespace Game
{
	void SDL_deleter::operator()(SDL_Window* window) const
	{
		SDL_DestroyWindow(window);
	}

	void SDL_deleter::operator()(SDL_Renderer* renderer) const
	{
		SDL_DestroyRenderer(renderer);
	}

	void SDL_deleter::operator()(SDL_Texture* texture) const
	{
		SDL_DestroyTexture(texture);
	}

	void SDL_deleter::operator()(TTF_Font* font) const
	{
		TTF_CloseFont(font);
	}

	void SDL_deleter::operator()(char* text) const
	{
		SDL_free(text);
	}

	void SDL_deleter::operator()(SDL_Surface* surface) const
	{
		SDL_FreeSurface(surface);
	}

	void SDL_deleter::operator()(SDL_GameController* controller) const
	{
		SDL_GameControllerClose(controller);
	}
}

#pragma once

#include <SDL.h>
#include <Shapes.h>
#include "Colors.h"
#include "Fonts.h"
#include "Sound.h"


namespace Game
{
	namespace Wrappers
	{
		class Sdl_wrapper
		{
		public:
			Sdl_wrapper();
			~Sdl_wrapper();

			static SDL_Rect get_rect(const Geometry::Rectangle<int>& rect);
			static SDL_Point get_point(const Geometry::Vector<int>& point);
			static SDL_Color get_color(const Graphics::Color color);
			static TTF_Font& get_font( const Text::Font& font);
			static Mix_Chunk& get_sound_chunk(const Audio::Sound_effect& sound);
			static Mix_Music& get_music(const Audio::Music_track& music);

		private:
			static int count_;
		};
	}
}

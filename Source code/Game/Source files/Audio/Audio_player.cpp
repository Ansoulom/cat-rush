#include "Audio_player.h"
#include "SDL_wrapper.h"


namespace Game
{
	namespace Audio
	{
		void Audio_player::play_sound(Sound_effect& sound) const
		{
			if(Mix_PlayChannel(-1, &Wrappers::Sdl_wrapper::get_sound_chunk(sound), 0) == -1)
			{
				throw std::runtime_error{std::string{"Could not play sound: "} + Mix_GetError()};
			}
		}


		void Audio_player::play_music(Music_track& music) const
		{
			if(Mix_PlayMusic(&Wrappers::Sdl_wrapper::get_music(music), -1))
			{
				throw std::runtime_error{std::string{"Could not play music: "} + Mix_GetError()};
			}
		}
	}
}

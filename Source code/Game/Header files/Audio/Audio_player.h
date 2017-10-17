#pragma once

#include <SDL_mixer.h>
#include "Sound.h"


namespace Game
{
	namespace Audio
	{
		class Audio_player
		{
		public:
			void play_sound(Sound_effect& sound) const;
			void play_music(Music_track& music) const;
		};
	}
}

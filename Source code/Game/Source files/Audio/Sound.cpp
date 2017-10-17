#include "Sound.h"


namespace Game
{
	namespace Audio
	{
		Sound_effect::Sound_effect(const boost::filesystem::path& file_path)
			: sdl_sound_{Mix_LoadWAV(file_path.string().c_str())}
		{
			if(!sdl_sound_)
			{
				throw std::runtime_error{std::string{"Could not load sound effect: "} + Mix_GetError()};
			}
		}


		Music_track::Music_track(const boost::filesystem::path& file_path)
			: sdl_music_{Mix_LoadMUS(file_path.string().c_str())}
		{
			if(!sdl_music_)
			{
				throw std::runtime_error{std::string{"Could not load music file: "} + Mix_GetError()};
			}
		}
	}
}

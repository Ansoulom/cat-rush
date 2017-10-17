#pragma once
#include <memory>
#include <SDL_mixer.h>
#include "SDL_deleter.h"
#include <boost/filesystem/path.hpp>


namespace Game
{
	namespace Wrappers
	{
		class Sdl_wrapper;
	}


	namespace Audio
	{
		class Sound_effect
		{
		public:
			explicit Sound_effect(const boost::filesystem::path& file_path);

		private:
			std::unique_ptr<Mix_Chunk, Sdl_deleter> sdl_sound_;

			friend class Wrappers::Sdl_wrapper;
		};


		class Music_track
		{
		public:
			explicit Music_track(const boost::filesystem::path& file_path);

		private:
			std::unique_ptr<Mix_Music, Sdl_deleter> sdl_music_;

			friend class Wrappers::Sdl_wrapper;
		};
	}
}

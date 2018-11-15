#include "SDL_wrapper.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Logger.h"
#include <memory>
#include "SDL_deleter.h"
#include <filesystem>


namespace Game
{
	namespace Wrappers
	{
		// TODO: Make it a singleton instead, this is just silly
		Sdl_wrapper::Sdl_wrapper()
		{
			if(count_++ == 0)
			{
				if(SDL_Init(SDL_INIT_EVERYTHING))
				{
					throw std::runtime_error{std::string{"Could not initiate SDL: "} + SDL_GetError()};
				}
				if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
				{
					Logger::log("Warning: Linear texture filtering not enabled!");
				}
				if(!IMG_Init(IMG_INIT_PNG))
				{
					SDL_Quit();
					throw std::runtime_error{std::string{"Could not initiate SDL_image: "} + IMG_GetError()};
				}
				if(TTF_Init())
				{
					SDL_Quit();
					IMG_Quit();
					throw std::runtime_error{std::string{"Could not initiate SDL_ttf: "} + TTF_GetError()};
				}
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					SDL_Quit();
					IMG_Quit();
					TTF_Quit();
					throw std::runtime_error{std::string{"Could not initiate SDL_ttf: "} + Mix_GetError()};
				}
				auto path = std::unique_ptr<char, Sdl_deleter>{SDL_GetBasePath(), Sdl_deleter{}};
				// Would prefer std::filesystem::current_path, but doesn't return consistent path
				std::filesystem::current_path(path.get());
			}
		}


		Sdl_wrapper::~Sdl_wrapper()
		{
			if(--count_ == 0)
			{
				SDL_Quit();
				IMG_Quit();
				TTF_Quit();
				Mix_Quit();
			}
		}


		SDL_Rect Sdl_wrapper::get_rect(const Geometry::Rectangle<int>& rect)
		{
			return {rect.left(), rect.top(), rect.width(), rect.height()};
		}


		SDL_Point Sdl_wrapper::get_point(const Geometry::Vector<int>& point)
		{
			return {point.get_x(), point.get_y()};
		}


		SDL_Color Sdl_wrapper::get_color(const Graphics::Color color)
		{
			return {color.red(), color.green(), color.blue(), color.alpha()};
		}


		TTF_Font& Sdl_wrapper::get_font(const Text::Font& font)
		{
			return *font.sdl_font_;
		}


		Mix_Chunk& Sdl_wrapper::get_sound_chunk(const Audio::Sound_effect& sound)
		{
			return *sound.sdl_sound_;
		}


		Mix_Music& Sdl_wrapper::get_music(const Audio::Music_track& music)
		{
			return *music.sdl_music_;
		}


		int Sdl_wrapper::count_ = 0;
	}
}

#include "Game_core.h"
#include "Timer.h"
#include <boost/filesystem.hpp>
#include "Frame_timer.h"
#include "Component.h"


namespace Game
{
	Game_core::Game_core(std::string name) : sdl_wrapper_{}, renderer_{}, collisions_{}, name_ {name}, running_{false}, input_{},
											 window_{}, sdl_renderer_{}, font_{}, use_vsync_{true}, texture_manager_{}, world_{}
	{
		window_ = std::unique_ptr<SDL_Window, Sdl_deleter>{
			SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
							 SDL_WINDOW_HIDDEN),
			Sdl_deleter{}
		};
		if(!window_)
		{
			throw std::runtime_error{std::string{"Window could not be created: "} + SDL_GetError()};
		}

		sdl_renderer_ = std::unique_ptr<SDL_Renderer, Sdl_deleter>{
			SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | (use_vsync_ ? SDL_RENDERER_PRESENTVSYNC : 0)),
			Sdl_deleter{}
		};
		if(!sdl_renderer_)
		{
			throw std::runtime_error{std::string{"Renderer could not be created! SDL Error: "} + SDL_GetError()};
		}

		font_ = std::unique_ptr<TTF_Font, Sdl_deleter>{TTF_OpenFont("Resources/Fonts/zsynorEBO.ttf", 72), Sdl_deleter{}};
		texture_manager_.load_all_textures(boost::filesystem::path{"Resources/Textures/"}, *sdl_renderer_);
	}


	Game_core::~Game_core() { }


	void Game_core::run()
	{
		SDL_ShowWindow(window_.get());

		auto loader = Objects::Component_loader{renderer_, collisions_};
		world_ = std::unique_ptr<World>{load_world("LevelFileTest", loader)};

		running_ = true;

		auto frame_timer = Frame_timer{};
		while(running_)
		{
			auto time_passed = frame_timer.update();
			auto fps = frame_timer.get_framerate();
			SDL_SetWindowTitle(window_.get(), (std::string{"Projekt Tidshax    FPS: "} + std::to_string(fps)).c_str());

			handle_events(time_passed);
			update(time_passed);
			render();
		}
	}


	TTF_Font& Game_core::get_font() const
	{
		return *font_.get();
	}


	int Game_core::convert_meters_to_pixels(double meters)
	{
		return static_cast<int>(round(meters * pixels_per_meter));
	}


	double Game_core::convert_pixels_to_meters(int pixels)
	{
		return pixels / static_cast<double>(pixels_per_meter);
	}


	void Game_core::handle_events(Timer::Seconds time_passed)
	{
		input_.update(time_passed);
		auto event = SDL_Event{};
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running_ = false;
					break;
				default:
					input_.handle_event(event);
					break;
			}
		}
	}


	void Game_core::update(Timer::Seconds time_passed)
	{
		world_->handle_input(time_passed, input_);
		world_->update(time_passed);
	}


	void Game_core::render()
	{
		SDL_SetRenderDrawColor(sdl_renderer_.get(), 0x00, 0x00, 0x00, 0xff);
		SDL_RenderClear(sdl_renderer_.get());

		// TODO: Not sure if this is the best way to do this, look up alternatives.
		SDL_RenderSetScale(sdl_renderer_.get(), screen_width / static_cast<float>(source_width),
						   screen_height / static_cast<float>(source_height));

		renderer_.render(texture_manager_, world_->camera, *sdl_renderer_);

		SDL_RenderSetScale(sdl_renderer_.get(), 1, 1);

		// Render GUI here maybe?

		SDL_RenderPresent(sdl_renderer_.get());
	}
}

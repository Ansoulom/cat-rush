#include "Game_core.h"
#include "Timer.h"
#include <boost/filesystem.hpp>
#include "Frame_timer.h"
#include "Component.h"


namespace Game
{
	Game_core::Game_core(std::string name) : sdl_wrapper{}, renderer{}, collisions{}, name {name}, running{false}, input{},
											 window{}, sdl_renderer{}, font{}, use_vsync{true}, texture_manager{}, world{}
	{
		window = std::unique_ptr<SDL_Window, SDL_deleter>{
			SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
							 SDL_WINDOW_HIDDEN),
			SDL_deleter{}
		};
		if(!window)
		{
			throw std::runtime_error{std::string{"Window could not be created: "} + SDL_GetError()};
		}

		sdl_renderer = std::unique_ptr<SDL_Renderer, SDL_deleter>{
			SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | (use_vsync ? SDL_RENDERER_PRESENTVSYNC : 0)),
			SDL_deleter{}
		};
		if(!sdl_renderer)
		{
			throw std::runtime_error{std::string{"Renderer could not be created! SDL Error: "} + SDL_GetError()};
		}

		font = std::unique_ptr<TTF_Font, SDL_deleter>{TTF_OpenFont("Resources/Fonts/zsynorEBO.ttf", 72), SDL_deleter{}};
		texture_manager.load_all_textures(boost::filesystem::path{"Resources/Textures/"}, *sdl_renderer);
	}


	Game_core::~Game_core() { }


	void Game_core::run()
	{
		SDL_ShowWindow(window.get());

		auto loader = Objects::Component_loader{renderer, collisions};
		world = std::unique_ptr<World>{load_world("LevelFileTest", loader)};

		running = true;

		auto frame_timer = Frame_timer{};
		while(running)
		{
			auto time_passed = frame_timer.update();
			auto fps = frame_timer.get_framerate();
			SDL_SetWindowTitle(window.get(), (std::string{"Projekt Tidshax    FPS: "} + std::to_string(fps)).c_str());

			handle_events(time_passed);
			update(time_passed);
			render();
		}
	}


	TTF_Font& Game_core::get_font() const
	{
		return *font.get();
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
		input.update(time_passed);
		auto event = SDL_Event{};
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				default:
					input.handle_event(event);
					break;
			}
		}
	}


	void Game_core::update(Timer::Seconds time_passed)
	{
		world->handle_input(time_passed, input);
		world->update(time_passed);
	}


	void Game_core::render()
	{
		SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0x00, 0x00, 0xff);
		SDL_RenderClear(sdl_renderer.get());

		// TODO: Not sure if this is the best way to do this, look up alternatives.
		SDL_RenderSetScale(sdl_renderer.get(), screen_width / static_cast<float>(source_width),
						   screen_height / static_cast<float>(source_height));

		renderer.render(texture_manager, world->camera, *sdl_renderer);

		SDL_RenderSetScale(sdl_renderer.get(), 1, 1);

		// Render GUI here maybe?

		SDL_RenderPresent(sdl_renderer.get());
	}
}

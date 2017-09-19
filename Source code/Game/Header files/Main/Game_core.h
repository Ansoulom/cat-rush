#pragma once

#include <SDL_ttf.h>
#include <memory>
#include "Input_handler.h"
#include "World.h"
#include "Texture_manager.h"
#include "Timer.h"
#include "SDL_deleter.h"
#include "SDL_wrapper.h"
#include "Collision_system.h"

namespace Game
{
	class Game_core
	{
	public:
		explicit Game_core(std::string name);
		~Game_core();

		void run(); // Runs the main game loop.
		TTF_Font& get_font() const;

		static int convert_meters_to_pixels(double meters);
		static double convert_pixels_to_meters(int pixels);

		static constexpr int pixels_per_meter = 100;
		static constexpr int source_width = 3840;
		static constexpr int source_height = 2160;

		//TODO ÄNDRA SÅ ATT DET INTE ÄR KONSTANT
		static constexpr int screen_width = 1280;
		static constexpr int screen_height = 720;

	private:
		void handle_events(Timer::Seconds time_passed); // Handles all low-level events.
		void update(Timer::Seconds time_passed); // Should handle all game logic.
		void render(); // Render all game objects and GUI to the screen.

		SDL_wrapper sdl_wrapper;
		Graphics::Renderer renderer;
		Physics::Collision_system collisions;
		const std::string name;
		bool running;
		Input::Input_handler input;
		std::unique_ptr<SDL_Window, SDL_deleter> window;
		std::unique_ptr<SDL_Renderer, SDL_deleter> sdl_renderer;
		std::unique_ptr<TTF_Font, SDL_deleter> font;
		bool use_vsync;
		Graphics::Texture_manager texture_manager;
		std::unique_ptr<World> world;
	};
}
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

		Sdl_wrapper sdl_wrapper_;
		Graphics::Renderer renderer_;
		Physics::Collision_system collisions_;
		const std::string name_;
		bool running_;
		Input::Input_handler input_;
		std::unique_ptr<SDL_Window, Sdl_deleter> window_;
		std::unique_ptr<SDL_Renderer, Sdl_deleter> sdl_renderer_;
		std::unique_ptr<TTF_Font, Sdl_deleter> font_;
		bool use_vsync_;
		Graphics::Texture_manager texture_manager_;
		std::unique_ptr<World> world_;
	};
}
#pragma once

#include <memory>
#include "Input_handler.h"
#include "World.h"
#include "Timer.h"
#include "SDL_wrapper.h"
#include "Settings.h"


namespace Game
{
	class Game_core
	{
	public:
		explicit Game_core(std::string name);
		~Game_core();

		void run(); // Runs the main game loop.

		const Settings& settings() const;

	private:
		void handle_events(Timer::Seconds time_passed); // Handles all low-level events.
		void update(Timer::Seconds time_passed); // Should handle all game logic.
		void render(); // Render all game objects and GUI to the screen.

		Sdl_wrapper sdl_wrapper_;
		Settings settings_;
		bool running_;
		Input::Input_handler input_;
		std::unique_ptr<World> world_;

		static const std::string settings_file;
	};
}

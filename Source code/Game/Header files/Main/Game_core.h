#pragma once

#include <memory>
#include "Input_handler.h"
#include "World.h"
#include "Timer.h"
#include "SDL_wrapper.h"
#include "Window.h"
#include "Settings.h"
#include "Resource_managers.h"
#include "HUD.h"


namespace Game
{
	namespace Core
	{
		class Game_core
		{
		public:
			Game_core();

			void run(); // Runs the main game loop.

			const Settings& settings() const;

		private:
			void handle_events(Timer::Seconds time_passed); // Handles all low-level events.
			void update(Timer::Seconds time_passed); // Should handle all game logic.
			void render(); // Render all game objects and GUI to the screen.

			Wrappers::Sdl_wrapper sdl_wrapper_;
			Settings settings_;
			bool running_;
			Input::Input_handler input_;
			Resources::Resource_manager resources_;
			Window window_;
			Graphics::Renderer renderer_;
			std::unique_ptr<World> world_;
			Gui::Hud hud_;
		};
	}
}

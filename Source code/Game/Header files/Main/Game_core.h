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
	namespace Game_states
	{
		class Game_state
		{
		public:
			virtual ~Game_state();

			virtual void handle_input(const Timer::Seconds time_passed, const Input::Input_handler& input) = 0;
			virtual void update(const Timer::Seconds time_passed) = 0;
			virtual std::vector<Graphics::Render_instance> get_render_instances(const Resources::Texture_manager& tm, const Graphics::Render_settings& render_settings) const = 0;
		};
	}


	namespace Core
	{
		class Game_core
		{
		public:
			Game_core();

			void run(); // Runs the main game loop.

			void go_to_win_screen();
			void go_to_death_screen();
			void play_game();

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

			std::unique_ptr<Game_states::Game_state> state_;
		};
	}
}

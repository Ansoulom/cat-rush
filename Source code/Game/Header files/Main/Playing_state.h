#pragma once

#include "Game_core.h"


namespace Game
{
	namespace Game_states
	{
		class Playing_state : public Game_state
		{
		public:
			Playing_state(Core::Game_core& game_context, const std::string& world_name);

			// Overridden functions
			void handle_input(const Timer::Seconds time_passed, const Input::Input_handler& input) override;
			void update(const Timer::Seconds time_passed) override;
			std::vector<Graphics::Render_instance> get_render_instances(const Resources::Texture_manager& tm, const Graphics::Render_settings& render_settings) const override;

			void game_won();
			void game_lost();

		private:
			Core::Game_core* game_context_;
			Communication::Receiver<> boss_died_receiver_, player_died_receiver_;
			Graphics::Render_grid render_grid_{};
			std::unique_ptr<Core::World> world_;
			Gui::Hud hud_;
		};
	}
}

#pragma once

#include "Game_core.h"


namespace Game
{
	namespace Game_states
	{
		class Text_state : public Game_state
		{
		public:
			explicit Text_state(Graphics::Texture&& text_texture, Core::Game_core& game_context);

			void handle_input(const Timer::Seconds time_passed, const Input::Input_handler& input) override;
			void update(const Timer::Seconds time_passed) override;
			std::vector<Graphics::Render_instance> get_render_instances(const Resources::Texture_manager& tm, const Graphics::Render_settings& render_settings) const override;

		private:
			Core::Game_core* game_context_;
			Graphics::Texture texture_;
		};
	}
}

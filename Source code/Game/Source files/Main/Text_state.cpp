#include "Text_state.h"
#include "Logger.h"


namespace Game
{
	namespace Game_states
	{
		Text_state::Text_state(Graphics::Texture&& text_texture, Core::Game_core& game_context)
			: game_context_{&game_context}, texture_{std::move(text_texture)} {}


		void Text_state::handle_input(const Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			if(input.get_action(Input::Action::exit, "menu"))
			{
				Logger::log("exit button");
				game_context_->exit();
			}
			else if(input.get_action(Input::Action::start, "menu"))
			{
				Logger::log("play button");
				game_context_->play_game();
			}
		}


		void Text_state::update(const Timer::Seconds time_passed) {}


		std::vector<Graphics::Render_instance> Text_state::get_render_instances(
			const Resources::Texture_manager& tm,
			const Graphics::Render_settings& render_settings) const
		{
			return {
				{
					texture_,
					{
						{render_settings.render_size.get_x() / 2, render_settings.render_size.get_y() / 2},
						texture_.width(),
						texture_.height()
					},
					1
				}
			};
		}
	}
}

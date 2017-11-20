#include "Playing_state.h"
#include "Renderer.h"


namespace Game
{
	namespace Game_states
	{
		Playing_state::Playing_state(
			Core::Game_core& game_context,
			const std::string& world_name)
			: game_context_{&game_context},
			  boss_died_receiver_{std::bind(&Playing_state::game_won, this)},
			  player_died_receiver_{std::bind(&Playing_state::game_lost, this)},
			  world_{load_world(world_name, game_context, Objects::Component_loader{render_grid_})},
			  hud_ {
				  {game_context.settings().constants().source_width(), game_context.settings().constants().source_height()},
				  *world_
			  }
		{
			if (auto player = world_->find_object_by_name("player"))
			{
				if (const auto player_combat = player->find_component<Objects::Combat_component>())
					player_combat->add_died_receiver(player_died_receiver_);
			}
			if (auto boss = world_->find_object_by_name("boss"))
			{
				if (const auto boss_combat = boss->find_component<Objects::Combat_component>())
					boss_combat->add_died_receiver(boss_died_receiver_);
			}
		}


		void Playing_state::handle_input(const Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			world_->handle_input(time_passed, input);
		}


		void Playing_state::update(const Timer::Seconds time_passed)
		{
			world_->update(time_passed);
		}


		std::vector<Graphics::Render_instance> Playing_state::
			get_render_instances(const Resources::Texture_manager& tm, const Graphics::Render_settings& render_settings) const
		{
			return Graphics::merge_render_lists(
				{
					render_grid_.get_render_instances(tm, render_settings, world_->camera()),
					hud_.get_render_instances(tm, render_settings)
				});
		}


		void Playing_state::game_won()
		{
			game_context_->go_to_win_screen();
		}


		void Playing_state::game_lost()
		{
			game_context_->go_to_death_screen();
		}
	}
}

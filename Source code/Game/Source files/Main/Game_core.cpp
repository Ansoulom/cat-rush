#include "Game_core.h"
#include "Timer.h"
#include <boost/filesystem.hpp>
#include "Frame_timer.h"
#include "Component.h"
#include "File_paths.h"
#include "Logger.h"


namespace Game
{
	namespace Core
	{
		Game_core::Game_core()
			: sdl_wrapper_{},
			  settings_{Io::Paths::game_constants_file, Io::Paths::user_settings_file},
			  running_{false},
			  input_{},
			  resources_{},
			  window_{
				  settings_.constants().name(),
				  0,
				  0,
				  settings_.user_settings().resolution_width(),
				  settings_.user_settings().resolution_height(),
				  settings_.user_settings().fullscreen_mode(),
				  false
			  },
			  renderer_{settings_, window_},
			  world_{},
			  hud_{{settings_.constants().source_width(), settings_.constants().source_height()}},
			  boss_died_receiver_{std::bind(&Game_core::go_to_win_screen, this)},
			  player_died_receiver_{std::bind(&Game_core::go_to_death_screen, this)}
		{
			SDL_DisplayMode dm;
			if(SDL_GetCurrentDisplayMode(0, &dm) != 0)
				throw std::runtime_error{std::string{"Could not retrieve display mode: "} + SDL_GetError()};
			window_.set_position({dm.w / 2 - window_.get_size().get_x() / 2, dm.h / 2 - window_.get_size().get_y() / 2});
			resources_.textures().load_all_textures(boost::filesystem::path{Io::Paths::textures}, renderer_);
		}


		void Game_core::run()
		{
			window_.set_visible(true);

			const auto loader = Objects::Component_loader{renderer_};
			world_ = std::unique_ptr<World>{load_world("LevelFileTest", *this, loader)};
			add_world_event_receivers();

			running_ = true;

			auto frame_timer = Frame_timer{};
			while(running_)
			{
				const auto time_passed = frame_timer.update();
				const auto fps = frame_timer.get_framerate();
				const auto title = settings().constants().name() + std::string{"    FPS: "} + std::to_string(fps);
				window_.set_title(title);

				handle_events(time_passed);
				update(time_passed);
				render();
			}
		}


		const Settings& Game_core::settings() const
		{
			return settings_;
		}


		void Game_core::handle_events(Timer::Seconds time_passed)
		{
			input_.update(time_passed);
			auto event = SDL_Event{};
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_QUIT:
						running_ = false;
						break;
					default:
						input_.handle_event(event);
						break;
				}
			}
		}


		void Game_core::update(Timer::Seconds time_passed)
		{
			world_->handle_input(time_passed, input_);
			world_->update(time_passed);
		}


		void Game_core::render()
		{
			renderer_.clear({0x00, 0x00, 0x00, 0xff});

			// TODO: Not sure if this is the best way to do this, look up alternatives.
			renderer_.set_render_scale(
				settings_.user_settings().resolution_width() / static_cast<double>(settings_.constants().source_width()),
				settings_.user_settings().resolution_height() / static_cast<double>(settings_.constants().source_height()));

			renderer_.render(resources_.textures(), world_->camera());
			for(auto& instance : hud_.get_render_instances(resources_.textures(), renderer_.render_settings()))
			{
				renderer_.render(instance);
			}

			renderer_.set_render_scale(1.0, 1.0);

			// Render GUI here maybe?

			renderer_.show();
		}


		void Game_core::add_world_event_receivers()
		{
			hud_.connect_with_world(*world_);

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


		void Game_core::go_to_win_screen()
		{
			Logger::log("You won!");
		}


		void Game_core::go_to_death_screen()
		{
			Logger::log("You lost...");
		}
	}
}

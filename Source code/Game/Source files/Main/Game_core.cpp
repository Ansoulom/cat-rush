#include "Game_core.h"
#include "Timer.h"
#include <boost/filesystem.hpp>
#include "Frame_timer.h"
#include "Component.h"
#include "File_paths.h"


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
				  1920 / 2 /* TODO: Read screen size instead */,
				  1080 / 2,
				  settings_.user_settings().resolution_width(),
				  settings_.user_settings().resolution_height(),
				  settings_.user_settings().fullscreen_mode(),
				  false
			  },
			  renderer_{settings_, window_},
			  world_{}
		{
			resources_.textures().load_all_textures(boost::filesystem::path{Io::Paths::textures}, renderer_);
		}


		void Game_core::run()
		{
			window_.set_visible(true);

			auto loader = Objects::Component_loader{renderer_};
			world_ = std::unique_ptr<World>{load_world("LevelFileTest", *this, loader)};

			running_ = true;

			auto frame_timer = Frame_timer{};
			while(running_)
			{
				auto time_passed = frame_timer.update();
				auto fps = frame_timer.get_framerate();
				window_.set_title(settings().constants().name() +  std::string{"    FPS: "} + std::to_string(fps));

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

			renderer_.set_render_scale(1.0, 1.0);

			// Render GUI here maybe?

			renderer_.show();
		}
	}
}

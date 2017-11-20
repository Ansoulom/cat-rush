#include "Game_core.h"
#include "Timer.h"
#include <boost/filesystem.hpp>
#include "Frame_timer.h"
#include "Component.h"
#include "File_paths.h"
#include "Logger.h"
#include "Playing_state.h"
#include "Text_state.h"


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
			  state_{std::make_unique<Game_states::Playing_state>(*this, "LevelFileTest")}
		{
			SDL_DisplayMode dm;
			if(SDL_GetCurrentDisplayMode(0, &dm) != 0)
				throw std::runtime_error{std::string{"Could not retrieve display mode: "} + SDL_GetError()};
			window_.set_position({dm.w / 2 - window_.get_size().get_x() / 2, dm.h / 2 - window_.get_size().get_y() / 2});
			resources_.textures().load_all_textures(boost::filesystem::path{Io::Paths::textures}, renderer_);
			resources_.fonts().load_all_fonts(boost::filesystem::path{ Io::Paths::fonts });
		}


		void Game_core::run()
		{
			window_.set_visible(true);

			running_ = true;

			auto frame_timer = Frame_timer{};
			while(running_)
			{
				auto time_passed = frame_timer.update();
				const auto max_time = std::chrono::nanoseconds{std::chrono::seconds{1}} / 10; // TODO: Cleanup
				if(time_passed > max_time) time_passed = max_time;
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


		void Game_core::handle_events(const Timer::Seconds time_passed)
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


		void Game_core::update(const Timer::Seconds time_passed)
		{
			state_->handle_input(time_passed, input_);
			state_->update(time_passed);
		}


		void Game_core::render()
		{
			renderer_.clear({0x00, 0x00, 0x00, 0xff});

			// TODO: Not sure if this is the best way to do this, look up alternatives.
			renderer_.set_render_scale(
				settings_.user_settings().resolution_width() / static_cast<double>(settings_.constants().source_width()),
				settings_.user_settings().resolution_height() / static_cast<double>(settings_.constants().source_height()));

			//renderer_.render(resources_.textures(), world_->camera());
			for(auto& instance : state_->get_render_instances(resources_.textures(), renderer_.render_settings()))
			{
				renderer_.render(instance);
			}

			renderer_.set_render_scale(1.0, 1.0);

			// Render GUI here maybe?

			renderer_.show();
		}


		void Game_core::go_to_win_screen()
		{
			Logger::log("Player won!");
			/*
			state_ = std::make_unique<Game_states::Text_state>(
				Graphics::Texture{
					renderer_,
					"You won! Press ENTER to play again or ESCAPE to exit.",
					Graphics::Color{0xff, 0xff, 0xff},
					resources_.fonts().get_font("zsynorEBO")
				});*/ // TODO: Must fix so that this function is not called while the game is in the middle of updating
		}


		void Game_core::go_to_death_screen()
		{
			Logger::log("Player lost...");
			/*state_ = std::make_unique<Game_states::Text_state>(
				Graphics::Texture{
					renderer_,
					"You lost... Press ENTER to try again or ESCAPE to exit.",
					Graphics::Color{0xff, 0xff, 0xff},
					resources_.fonts().get_font("zsynorEBO")
				});*/ // TODO: Must fix so that this function is not called while the game is in the middle of updating
		}


		void Game_core::play_game()
		{
			state_ = std::make_unique<Game_states::Playing_state>(*this, "LevelFileTest");
		}
	}


	Game_states::Game_state::~Game_state() {}
}

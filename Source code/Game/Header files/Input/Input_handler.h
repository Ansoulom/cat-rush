#pragma once

#include <SDL.h>
#include "Vector.h"
#include <vector>
#include "Input_context.h"
#include "SDL_deleter.h"
#include "Timer.h"


namespace Game
{
	namespace Input
	{
		class Input_handler
		{
		public:
			Input_handler();
			~Input_handler();

			void set_active_contexts(std::initializer_list<std::string> context_names);
			void handle_event(const SDL_Event& event);
			void update(Timer::Seconds time_passed);

			bool get_action(Action action, std::string context) const;
			bool get_state(State state, std::string context) const;
			double get_range(Range range, std::string context) const;
			Geometry::Vector<double> get_processed_vector(Range x, Range y, std::string context) const;

		private:
			void initialize_contexts();
			void add_controller(int index);
			void remove_controller(SDL_JoystickID id);
			void activate_controller(SDL_GameController* controller);
			void activate_keyboard();
			void forward_event(const SDL_Event& event);
			void reset_controller_contexts();
			void reset_keyboard_contexts();

			std::unordered_map<std::string, Context> keyboard_contexts_, controller_contexts_;
			std::vector<std::string> active_contexts_;
			std::vector<std::unique_ptr<SDL_GameController, Sdl_deleter>> controllers_;
			SDL_GameController* active_controller_;
			bool controller_mode_;
			Timer::Seconds time_passed_;

			static Uint8 get_mouse_button_from_name(std::string name);
			static Axis get_controller_axis_from_name(std::string name);
			static Action get_action_from_name(std::string name);
			static State get_state_from_name(std::string name);
			static Range get_range_from_name(std::string name);
			static Axis get_mouse_axis_from_name(std::string name);
			static Axis get_mouse_wheel_axis_from_name(std::string name);

			static constexpr Sint32 hard_controller_deadzone = 10000;
			static constexpr Sint32 hard_mouse_deadzone = 2000;
			static constexpr Sint32 hard_wheel_deadzone = 0;
			static constexpr double vector_deadzone = 0.2;
		};
	}
}
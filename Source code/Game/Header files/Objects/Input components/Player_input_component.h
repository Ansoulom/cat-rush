#pragma once

#include "Input_component.h"
#include "Vector.h"
#include <json.hpp>


namespace Game
{
	namespace Objects
	{
		class Player_input_component :
			public Component
		{
		public:
			explicit Player_input_component(Game_object* container);

			//Player_input_component* clone() override

			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;

			static Player_input_component* from_json(const IO::json& json, Game_object* game_object, const Component_loader& loader);
			IO::json to_json() const override;

		private:
			void update_rotation(Geometry::Vector<double> input, Timer::Seconds time_passed, Game_object& object);

			Geometry::Vector<double> smooth_input;
			double angle, last_input_angle;
			int direction;

			const Timer::Seconds max_rotation_time = Timer::Seconds{0.2}; // The time it takes to rotate 180 degrees
			const Timer::Seconds rotation_input_smoothing_time = Timer::Seconds{0.1}; // The time it takes for rotation input to move from 0 to 1
		};
	}
}

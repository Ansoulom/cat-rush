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
			explicit Player_input_component(Game_object& game_object);

			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;

			static Player_input_component* from_json(const IO::json& json, Game_object& game_object);
			IO::json to_json() const override;

		private:
			void update_rotation(Geometry::Vector<double> input, Timer::Seconds time_passed, Game_object& object);
			Direction direction_;

			static const Deserializer add_to_map;
		};
	}
}

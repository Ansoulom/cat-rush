#pragma once

#include <json.hpp>
#include "Movement_component.h"
#include "Component.h"


namespace Game
{
	namespace Objects
	{
		class Player_input_component :
			public Component
		{
		public:
			explicit Player_input_component(Game_object& game_object, Movement_component& movement_component, double max_speed);

			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;

			static Player_input_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

		private:
			void update_rotation(Direction direction);
			Direction direction_;
			double max_speed_;

			Movement_component* movement_component_;

			static const Deserializer add_to_map;
		};
	}
}

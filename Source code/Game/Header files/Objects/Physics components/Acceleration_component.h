#pragma once

#include "Component.h"
#include "Movement_component.h"


namespace Game
{
	namespace Objects
	{
		class Acceleration_component : public Component
		{
		public:
			Acceleration_component(Game_object& game_object, Geometry::Vector<double> acceleration, Movement_component& movement_component);

			void update(Timer::Seconds time_passed) override;

			static Acceleration_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

		private:
			Geometry::Vector<double> acceleration_;
			Movement_component* movement_component_;

			static const Deserializer add_to_map;
		};
	}
}

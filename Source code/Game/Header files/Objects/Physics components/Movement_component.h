#pragma once

#include <Component.h>
#include <vector>
#include "Timer.h"
#include <json.hpp>


namespace Game
{
	namespace Objects
	{
		class Moving_physics_component :
			public Component
		{
		public:
			Moving_physics_component(Game_object* game_object, double max_speed);
			
			void update(Timer::Seconds time_passed) override;
			void receive(const Events::Message& message) override;

			void set_x_velocity(double velocity);
			void set_y_velocity(double velocity);
			double get_speed() const;

			static Moving_physics_component* from_json(const nlohmann::json& json, Game_object* game_object, const Component_loader& loader);
			nlohmann::json to_json() const override;

		private:
			template<typename T>
			void handle_event(const T& message){}
			void handle_event(const Events::Change_velocity_normalized& message);

			Geometry::Vector<double> velocity_{ 0, 0 };
			double max_speed_;
		};
	}
}

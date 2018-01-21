#pragma once

#include "Timer.h"
#include "JSON_extensions.h"
#include "Component.h"


namespace Game
{
	namespace Objects
	{
		class Movement_component : public Component
		{
		public:
			explicit Movement_component(Game_object& game_object);

			void update(Timer::Seconds time_passed) override;
			void receive(const Events::Message& message) override;

			Geometry::Vector<double>& velocity();
			Geometry::Vector<double> velocity() const;

			static Movement_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

		private:
			template<typename T>
			void handle_event(const T& message) {}


			void handle_event(const Events::Collided& message);

			Geometry::Vector<double> velocity_{0, 0};

			static const Deserializer add_to_map;
		};
	}
}

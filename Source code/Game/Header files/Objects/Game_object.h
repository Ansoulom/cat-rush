#pragma once

#include <memory>
#include "Timer.h"
#include <json.hpp>
#include <Vector.h>
#include <Component.h>
#include <Communication.h>


namespace Game
{
	namespace Objects
	{
		class Game_object
		{
		public:
			explicit Game_object(Geometry::Vector<double> position);
			~Game_object();

			Game_object(const Game_object& other) = delete;
			Game_object(Game_object&& other) noexcept = delete;

			Game_object& operator=(const Game_object& other) = delete;
			Game_object& operator=(Game_object&& other) noexcept = delete;

			void update(Timer::Seconds time_passed);
			Geometry::Vector<double> get_position() const;
			void set_position(Geometry::Vector<double> position);
			void send(const Events::Message& message);
			void add_component(std::unique_ptr<Component>&& component);
			void add_destroy_listener(std::function<void(Game_object&)> function);

			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);

			static Game_object* from_json(const nlohmann::json& json, const Component_loader& loader);
			nlohmann::json to_json() const;

		private:
			Geometry::Vector<double> position;
			std::vector<std::unique_ptr<Component>> components;
			Communication::Event<Game_object&> destroyed_event;
		};
	}
}

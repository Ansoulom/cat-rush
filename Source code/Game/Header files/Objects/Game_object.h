#pragma once

#include <memory>
#include "Timer.h"
#include <json.hpp>
#include <Vector.h>
#include <Component.h>
#include <Communication.h>
#include "World.h"


namespace Game
{
	namespace Objects
	{
		class Game_object
		{
		public:
			explicit Game_object(Geometry::Vector<double> position, World& world);
			~Game_object();

			Game_object(const Game_object& other) = delete;
			Game_object(Game_object&& other) noexcept = delete;
			Game_object& operator=(const Game_object& other) = delete;
			Game_object& operator=(Game_object&& other) noexcept = delete; // May not have to delete them anyway

			Geometry::Vector<double>& position();
			Geometry::Vector<double> position() const;
			void send(const Events::Message& message);
			void add_component(std::unique_ptr<Component>&& component);
			World& world();
			void add_destroy_listener(std::function<void(Game_object&)> function);

			// Only call from enclosing world
			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
			void update(Timer::Seconds time_passed);

			static Game_object* from_json(const IO::json& json, World& world);
			IO::json to_json() const;

		private:
			Geometry::Vector<double> position_;
			std::vector<std::unique_ptr<Component>> components_;
			World& world_;

			Communication::Event<Game_object&> destroyed_event_; // TODO: Make this more legit
		};
	}
}

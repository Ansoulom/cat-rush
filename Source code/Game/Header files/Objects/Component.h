#pragma once

#include <variant>
#include "json.hpp"
#include "Timer.h"
#include "Vector.h"
#include <functional>
#include "Collision_system.h"


namespace Game
{
	namespace Input
	{
		class Input_handler;
	}


	namespace Graphics
	{
		class Renderer;
	}


	namespace Physics
	{
		class Collision_system;
	}


	namespace Objects
	{
		class Component_loader;
		class Game_object;
		class Graphics_component;
		class Collider_component;


		enum class Direction
		{
			left = -1,
			right = 1
		};


		// Internal events only
		namespace Events
		{
			struct Position_changed
			{
				Geometry::Vector<double> start_position;
			};


			struct Object_moved
			{
				Geometry::Vector<double> start_position;
			};


			struct Change_velocity_normalized
			{
				Geometry::Vector<double> normalized_velocity;
			};


			struct Direction_changed
			{
				Direction direction;
			};


			using Message = std::variant<Object_moved, Change_velocity_normalized, Direction_changed, Position_changed>;
		}


		class Component
		{
		public:
			explicit Component(Game_object& game_object);
			virtual ~Component();

			Component(const Component&) = delete;
			Component(Component&&) = delete;
			Component& operator=(const Component&) = delete;
			Component& operator=(Component&&) = delete;

			Game_object& game_object();
			const Game_object& game_object() const;
			// Receive a message from the enclosing game object
			virtual void receive(const Events::Message& message);

			// Only call from enclosing game object
			virtual void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
			virtual void update(Timer::Seconds time_passed);

			static Component* from_json(const IO::json& j, Game_object& game_object);
			virtual IO::json to_json() const = 0;

		protected:
			class Deserializer
			{
			public:
				Deserializer(std::string class_name, std::function<Component*(const IO::json&, Game_object&)> factory);
			};


		private:
			Game_object* game_object_;

			// Do not use this during static deinitialization!
			static std::unordered_map<std::string, std::function<Component*(const IO::json&, Game_object&)>>&
			deserialization_table();
		};


		class Component_loader
		{
		public:
			explicit Component_loader(Graphics::Renderer& renderer);

			void register_component(Graphics_component& comp) const;

		private:
			Graphics::Renderer& renderer_;
		};
	}
}

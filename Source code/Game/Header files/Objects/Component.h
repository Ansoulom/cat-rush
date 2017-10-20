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
		class Component_type_map;


		enum class Direction_x
		{
			left = -1,
			right = 1
		};


		enum class Direction_y
		{
			up = -1,
			down = 1
		};

		enum class Axis
		{
			x, y, both
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
				Axis axis;
				Geometry::Vector<double> start_position;
			};


			struct Direction_changed
			{
				Direction_x direction;
			};


			struct Collided
			{
				Collided(Axis axis, Collider_component& moving, Collider_component& collided_with, std::string layer);

				Axis axis;
				Collider_component& moving;
				Collider_component& collided_with;
				std::string layer;
			};


			using Message = std::variant<Object_moved, Direction_changed, Position_changed, Collided>;
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

			static Component* from_json(
				const Io::json& j,
				Game_object& game_object,
				const Component_type_map& component_map);
			virtual Io::json to_json() const = 0;
			virtual std::string component_type() const = 0;

		protected:
			class Deserializer
			{
			public:
				Deserializer(
					std::string class_name,
					std::function<Component*(const Io::json&, Game_object&, const Component_type_map&)> factory);
			};


		private:
			Game_object* game_object_;

			// Do not use this during static deinitialization!
			static std::unordered_map<std::string, std::function<Component*(
										  const Io::json&,
										  Game_object&,
										  const Component_type_map&)>>&
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


		class Component_type_map
		{
		public:
			template<typename T>
			T& get_component() const;
			Component& get_component(const std::string& type) const;
			bool contains(const std::string& type) const;
			void add_component(Component& component);
			 
		private:
			std::unordered_map<std::string, Component*> map_;
		};


		template<typename T>
		T& Component_type_map::get_component() const
		{
			return *dynamic_cast<T*>(map_.at(T::type()));
		}
	}
}

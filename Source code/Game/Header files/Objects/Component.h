#pragma once

#include <json.hpp>
#include <Timer.h>
#include <Vector.h>
#include <variant>


namespace Game
{
	namespace Input {
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


			struct Rotation_changed
			{
				double new_rotation;
			};


			using Message = std::variant<Object_moved, Change_velocity_normalized, Rotation_changed, Position_changed>;
		}


		class Component
		{
		public:
			explicit Component(Game_object* container);
			virtual ~Component();

			Component(const Component&) = delete;
			Component(Component&&) = delete;
			Component& operator=(const Component&) = delete;
			Component& operator=(Component&&) = delete;

			//virtual Component* clone() = 0; // The pointer is owned by and needs to be destroyed by the caller.

			virtual void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
			virtual void update(Timer::Seconds time_passed);
			virtual void receive(const Events::Message& message); // Receive a message from the enclosing game object
			Geometry::Vector<double> get_position() const;

			static Component* from_json(const nlohmann::json& j, Game_object* game_object, const Component_loader& loader);
			virtual nlohmann::json to_json() const = 0;

		protected:
			Game_object* game_object_;
		};


		class Component_loader
		{
		public:
			class Physics_loader
			{
			public:
				explicit Physics_loader(Physics::Collision_system& collision_system);

				void register_component(Collider_component& comp) const;
			private:
				Physics::Collision_system& collision_system_;

				friend class Collider_component;
			};

			Component_loader(Graphics::Renderer& renderer, Physics::Collision_system& collision_system);

			const Physics_loader& physics_loader() const;
			void register_component(Graphics_component& comp) const;
			void register_component(Collider_component& comp) const;

		private:
			Graphics::Renderer& renderer_;
			Physics_loader phys_loader_;
		};
	}
}

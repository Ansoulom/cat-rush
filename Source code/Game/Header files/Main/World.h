#pragma once

#include <vector>
#include "Timer.h"
#include "Game_object.h"
#include "Camera.h"
#include "Collision_system.h"
#include "Component.h"


namespace Game
{
	namespace Input
	{
		class Input_handler;
	}


	namespace Core
	{
		class Game_core;


		class World
		{
		public:
			World(Game_core& game_context, Objects::Component_loader component_loader);

			// Delete to avoid updating pointers
			World(const World& other) = delete;
			World(World&& other) = delete;
			World& operator=(const World& other) = delete;
			World& operator=(World&& other) = delete;

			Objects::Game_object& add_object_from_prefab(const std::string& prefab_name);
			Objects::Game_object& add_object_from_prefab(const std::string& prefab_name, Geometry::Vector<double> position);
			void add_object(std::unique_ptr<Objects::Game_object>&& object);
			void destroy_object(Objects::Game_object& object);
			const Objects::Component_loader& component_loader() const;
			Physics::Collision_system& collision_system();
			const Camera& camera() const;
			Objects::Game_object* find_object_by_name(const std::string& name);
			const Objects::Game_object* find_object_by_name(const std::string& name) const;

			// Should only be used from main loop
			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
			void update(Timer::Seconds time_passed);

			static World* from_json(
				const Io::json& json,
				Game_core& game_context,
				const Objects::Component_loader& component_loader);
			Io::json to_json() const;

		private:
			void add_new_objects();
			void remove_destroyed_objects();

			Physics::Collision_system collision_system_;
			const Objects::Component_loader component_loader_;
			Camera camera_;
			std::vector<std::unique_ptr<Objects::Game_object>> objects_;
			std::vector<std::unique_ptr<Objects::Game_object>> to_add_;
			std::unordered_map<std::string, Objects::Game_object*> object_name_map_;
			Game_core& game_context_;
			
		};


		// Name must correspond to a json file (without extension) in the World folder
		World* load_world(
			const std::string& name,
			Game_core& game_context,
			const Objects::Component_loader& component_loader);
	}
}

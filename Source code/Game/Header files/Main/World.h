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

			void add_object(std::unique_ptr<Objects::Game_object>&& object);
			const Objects::Component_loader& component_loader() const;
			Physics::Collision_system& collision_system();
			const Camera& camera() const;

			// Should only be used from main loop
			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
			void update(Timer::Seconds time_passed);

			static World* from_json(
				const Io::json& json,
				Game_core& game_context,
				const Objects::Component_loader& component_loader);
			Io::json to_json() const;

		private:
			std::vector<std::unique_ptr<Objects::Game_object>> objects_;
			Physics::Collision_system collision_system_;
			const Objects::Component_loader component_loader_;
			Game_core& game_context_;
			Camera camera_;
		};


		// Name must correspond to a json file (without extension) in the World folder
		World* load_world(
			const std::string& name,
			Game_core& game_context,
			const Objects::Component_loader& component_loader);
	}
}

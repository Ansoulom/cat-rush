#pragma once

#include <vector>
#include "Timer.h"
#include "Game_object.h"
#include <Camera.h>
#include <unordered_map>
#include <Renderer.h>


namespace Game
{
	namespace Input
	{
		class Input_handler;
	}


	class World
	{
	public:
		World();

		World(const World& other) = delete;
		World(World&& other) = delete;
		World& operator=(const World& other) = delete;
		World& operator=(World&& other) = delete;

		void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
		void update(Timer::Seconds time_passed);
		void add_object(std::unique_ptr<Objects::Game_object>&& object);

		static World* from_json(const IO::json& json, const Objects::Component_loader& loader);
		IO::json to_json() const;

		Camera camera; // ONLY TEMOPORARILY PUBLIC

	private:
		std::vector<std::unique_ptr<Objects::Game_object>> objects;
	};


	World* load_world(const std::string& name, Objects::Component_loader& loader); // Name must correspond to a json file (without extension) in the World folder
}

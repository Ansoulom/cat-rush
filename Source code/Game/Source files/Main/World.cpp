#include "World.h"
#include "Game_object.h"
#include "Camera.h"
#include "Game_core.h"
#include "json.hpp"
#include <fstream>


namespace Game
{
	World::World() : camera {{0.0, 0.0}}, objects{}
	{
	}


	/*
	World& World::operator=(const World& other)
	{
		objects = other.objects;
		grid.clear();
		for (auto& object : objects) // RISKY BUSINESS,
		{
			add_to_grid(object);
		}
		camera = other.camera;

		return *this;
	}/**/

	void World::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
	{
		for(auto& object : objects)
		{
			object->handle_input(time_passed, input);
		}
	}


	void World::update(Timer::Seconds time_passed)
	{
		for(auto& object : objects)
		{
			object->update(time_passed);
		}
		camera.update(time_passed);
	}


	World* World::from_json(const nlohmann::json& json, const Objects::Component_loader& loader)
	{
		auto world = new World{};
		auto objects_json = json.at("objects");
		for(auto& object_json : objects_json)
		{
			world->add_object(std::unique_ptr<Objects::Game_object>{Objects::Game_object::from_json(object_json, loader)});
		}
		// TODO: Set camera
		return world;
	}


	nlohmann::json World::to_json() const
	{
		auto objects_json = IO::json{};
		for(auto& object : objects)
		{
			objects_json.push_back(object->to_json());
		}
		return {"objects", objects_json};
	}


	void World::add_object(std::unique_ptr<Objects::Game_object>&& object)
	{
		objects.push_back(move(object));
	}


	World* load_world(const std::string& name, Objects::Component_loader& loader)
	{
		auto file = boost::filesystem::path{std::string{"World/"} + name + std::string{".json"}};
		std::ifstream in{file.string()};
		if(!in.is_open())
		{
			// TODO: Use a better exception
			throw std::runtime_error{std::string{"Could not open level file"} + file.string()};
		}

		auto world_json = IO::json{};
		in >> world_json;

		return World::from_json(world_json, loader);
	}
}

#include "World.h"
#include "Game_object.h"
#include "Camera.h"
#include "Game_core.h"
#include "json.hpp"
#include <fstream>


namespace Game
{
	World::World(Game_core& game_context, const Objects::Component_loader component_loader)
		: objects_{},
		  component_loader_{component_loader},
		  game_context_{game_context},
		  camera_ {
			  {0.0, 0.0},
			  Graphics::Aspect_ratio{
				  game_context.settings().source_width(),
				  game_context.settings().source_height()
			  }
		  } { }


	void World::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
	{
		for(auto& object : objects_)
		{
			object->handle_input(time_passed, input);
		}
	}


	void World::update(Timer::Seconds time_passed)
	{
		for(auto& object : objects_)
		{
			object->update(time_passed);
		}
		camera_.update(time_passed);
	}


	World* World::from_json(const IO::json& json, Game_core& game_context, const Objects::Component_loader& component_loader)
	{
		auto world = new World{game_context, component_loader};
		auto objects_json = json.at("objects");
		for(auto& object_json : objects_json)
		{
			world->add_object(std::unique_ptr<Objects::Game_object>{Objects::Game_object::from_json(object_json, world)});
		}
		// TODO: Set camera
		return world;
	}


	IO::json World::to_json() const
	{
		auto objects_json = IO::json{};
		for(auto& object : objects_)
		{
			objects_json.push_back(object->to_json());
		}
		return {"objects", objects_json};
	}


	void World::add_object(std::unique_ptr<Objects::Game_object>&& object)
	{
		objects_.push_back(move(object));
	}


	const Objects::Component_loader& World::component_loader() const
	{
		return component_loader_;
	}


	Physics::Collision_system& World::collision_system()
	{
		return collision_system_;
	}


	World* load_world(const std::string& name, Game_core& game_context, const Objects::Component_loader& component_loader)
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

		return World::from_json(world_json, game_context, component_loader);
	}
}

#include "World.h"
#include "Game_object.h"
#include "Camera.h"
#include "Game_core.h"
#include <fstream>
#include "Settings.h"
#include "JSON_extensions.h"


namespace Game
{
	namespace Core
	{
		World::World(Game_core& game_context, const Objects::Component_loader component_loader)
			: component_loader_{component_loader},
			  camera_{
				  {0.0, 0.0},
				  Graphics::Aspect_ratio{
					  game_context.settings().constants().source_width(),
					  game_context.settings().constants().source_height()
				  }
			  },
			  objects_{},
			  game_context_{game_context} { }


		void World::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			add_new_objects();
			for(auto& object : objects_)
			{
				if(!object->destroyed_)
					object->handle_input(time_passed, input);
			}
			remove_destroyed_objects();
		}


		void World::update(Timer::Seconds time_passed)
		{
			add_new_objects();
			for(auto& object : objects_)
			{
				if(!object->destroyed_)
					object->update(time_passed);
			}
			remove_destroyed_objects();

			camera_.update(time_passed);
		}


		World* World::from_json(
			const Io::json& json,
			Game_core& game_context,
			const Objects::Component_loader& component_loader)
		{
			auto world = new World{game_context, component_loader};
			auto objects_json = json.at("objects");
			for(auto& object_json : objects_json)
			{
				world->add_object(std::unique_ptr<Objects::Game_object>{Objects::Game_object::from_json(object_json, *world)});
			}
			return world;
		}


		Io::json World::to_json() const
		{
			auto objects_json = Io::json{};
			for(auto& object : objects_)
			{
				objects_json.push_back(object->to_json());
			}
			return {"objects", objects_json};
		}


		void World::add_new_objects()
		{
			for(auto& object : to_add_)
			{
				objects_.push_back(move(object));
			}
			to_add_.clear();
		}


		void World::remove_destroyed_objects()
		{
			for(auto i = 0; i < objects_.size(); ++i)
			{
				if(objects_[i]->destroyed_)
				{
					objects_.erase(objects_.begin() + i);
					--i;
				}
			}
		}


		Objects::Game_object& World::add_object_from_prefab(const std::string& prefab_name)
		{
			const auto object_json = Io::load_json_from_file({std::string{"Prefabs/"} + prefab_name + ".json"});
			auto obj_ptr = std::unique_ptr<Objects::Game_object>{
				Objects::Game_object::from_json(object_json, *this)
			};
			auto& object = *obj_ptr;
			add_object(move(obj_ptr));

			return object;
		}


		Objects::Game_object& World::
			add_object_from_prefab(const std::string& prefab_name, const Geometry::Vector<double> position)
		{
			const auto object_json = Io::load_json_from_file({std::string{"Prefabs/"} + prefab_name + ".json"});
			auto obj_ptr = std::unique_ptr<Objects::Game_object>{
				Objects::Game_object::from_json(object_json, *this, position)
			};
			auto& object = *obj_ptr;
			add_object(move(obj_ptr));

			return object;
		}


		void World::add_object(std::unique_ptr<Objects::Game_object>&& object)
		{
			if(object->name()) object_name_map_.emplace(object->name().value(), object.get());

			// Add to a temporary list until it can be put into the regular list
			to_add_.push_back(move(object));
		}


		void World::destroy_object(Objects::Game_object& object)
		{
			object.destroy();
			if(object.name())
			{
				object_name_map_.erase(object_name_map_.find(object.name().value()));
			}
		}


		const Objects::Component_loader& World::component_loader() const
		{
			return component_loader_;
		}


		Physics::Collision_system& World::collision_system()
		{
			return collision_system_;
		}


		const Camera& World::camera() const
		{
			return camera_;
		}


		Objects::Game_object* World::find_object_by_name(const std::string& name)
		{
			const auto iter = object_name_map_.find(name);
			return iter == object_name_map_.end() ? nullptr : iter->second;
		}


		const Objects::Game_object* World::find_object_by_name(const std::string& name) const
		{
			const auto iter = object_name_map_.find(name);
			return iter == object_name_map_.end() ? nullptr : iter->second;
		}


		World* load_world(const std::string& name, Game_core& game_context, const Objects::Component_loader& component_loader)
		{
			return World::from_json(Io::load_json_from_file({std::string{"World/"} + name + std::string{".json"}}), game_context,
									component_loader);
		}
	}
}

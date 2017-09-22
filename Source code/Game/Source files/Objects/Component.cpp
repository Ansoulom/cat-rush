#include "Component.h"
#include "Game_object.h"
#include "Renderer.h"
#include "Collision_system.h"
#include <Static_graphics_component.h>
#include <Animated_graphics_component.h>
#include <Player_input_component.h>
#include <Movement_component.h>
#include <Collider_component.h>


namespace Game
{
	namespace Objects
	{
		Component::Component(Game_object* container) : game_object_{container}
		{
		}


		Component::~Component()
		{
		}


		void Component::handle_input(Timer::Seconds, const Input::Input_handler&)
		{
		}


		void Component::update(Timer::Seconds)
		{
		}


		void Component::receive(const Events::Message&)
		{
		}


		Geometry::Vector<double> Component::get_position() const
		{
			return game_object_ ? game_object_->get_position() : Geometry::Vector<double>{};
		}


		Component* Component::from_json(const IO::json& j, Game_object* game_object, const Component_loader& loader)
		{
			auto type = j.at("type").get<std::string>();
			if(type == "Animated_graphics_component")
			{
				return Animated_graphics_component::from_json(j, game_object, loader);
			}
			if(type == "Static_graphics_component")
			{
				return Static_graphics_component::from_json(j, game_object, loader);
			}
			if(type == "Player_input_component")
			{
				return Player_input_component::from_json(j, game_object, loader);
			}
			if(type == "Movement_component")
			{
				return Moving_physics_component::from_json(j, game_object, loader);
			}
			if(type == "Collider_component")
			{
				return Collider_component::from_json(j, game_object, loader);
			}
			throw std::runtime_error{"Incorrect type when parsing compont from json"};
		}


		Component_loader::Component_loader(Graphics::Renderer& renderer, Physics::Collision_system& collision_system) : renderer_{renderer}, phys_loader_{collision_system}
		{
		}


		const Component_loader::Physics_loader& Component_loader::physics_loader() const
		{
			return phys_loader_;
		}


		void Component_loader::register_component(Graphics_component& comp) const
		{
			renderer_.register_component(comp);
		}


		void Component_loader::register_component(Collider_component& comp) const
		{
			phys_loader_.register_component(comp);
		}


		Component_loader::Physics_loader::Physics_loader(Physics::Collision_system& collision_system) : collision_system_{collision_system}
		{
		}


		void Component_loader::Physics_loader::register_component(Collider_component& comp) const
		{
			collision_system_.register_component(comp);
		}
	}
}

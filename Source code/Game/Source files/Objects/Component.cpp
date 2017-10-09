#include "Component.h"
#include "Game_object.h"
#include "Renderer.h"
#include "Collision_system.h"
#include "Static_graphics_component.h"
#include "Animated_graphics_component.h"
#include "Player_input_component.h"
#include "Movement_component.h"
#include "Collider_component.h"


namespace Game
{
	namespace Objects
	{
		Component::Component(Game_object& game_object) : game_object_{&game_object} { }


		Component::~Component() { }


		Game_object& Component::game_object()
		{
			return *game_object_;
		}


		const Game_object& Component::game_object() const
		{
			return *game_object_;
		}


		void Component::receive(const Events::Message&) { }


		void Component::handle_input(Timer::Seconds, const Input::Input_handler&) { }


		void Component::update(Timer::Seconds) { }


		Component* Component::from_json(const IO::json& j, Game_object& game_object)
		{
			const auto type = j.at("type").get<std::string>();
			return deserialization_table_.at(type)(j, game_object);
		}


		Component::Deserializer::Deserializer(
			std::string class_name,
			std::function<Component*(const IO::json&, Game_object&)> factory)
		{
			deserialization_table_.emplace(class_name, factory);
		}


		Component_loader::Component_loader(Graphics::Renderer& renderer, Physics::Collision_system& collision_system)
			: renderer_{renderer}, collision_system_{collision_system} { }


		void Component_loader::register_component(Graphics_component& comp) const
		{
			renderer_.register_component(comp);
		}


		void Component_loader::register_component(Collider_component& comp) const
		{
			collision_system_.register_component(comp);
		}
	}
}

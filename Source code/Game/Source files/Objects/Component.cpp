#include "Component.h"
#include "Game_object.h"
#include "Renderer.h"


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
			return deserialization_table().at(type)(j, game_object);
		}


		Component::Deserializer::Deserializer(
			std::string class_name,
			std::function<Component*(const IO::json&, Game_object&)> factory)
		{
			deserialization_table().emplace(class_name, factory);
		}


		std::unordered_map<std::string, std::function<Component*(const IO::json&, Game_object&)>>& Component::
			deserialization_table()
		{
			static auto table = std::unordered_map<std::string, std::function<Component*(const IO::json&, Game_object&)>>{};
			return table;
		}


		Component_loader::Component_loader(Graphics::Renderer& renderer)
			: renderer_{renderer} { }


		void Component_loader::register_component(Graphics_component& comp) const
		{
			renderer_.register_component(comp);
		}
	}
}

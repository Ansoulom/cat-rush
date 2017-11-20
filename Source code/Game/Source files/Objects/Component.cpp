#include "Component.h"
#include "Game_object.h"
#include "Renderer.h"


namespace Game
{
	namespace Objects
	{
		namespace Events
		{
			Collided::Collided(Axis axis, Direction movement_direction, Collider_component& moving, Collider_component& collided_with, std::string layer)
				: axis{axis}, movement_direction{movement_direction}, moving{moving}, collided_with{collided_with}, layer{layer} {}
		}


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


		Component* Component::from_json(
			const Io::json& j,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			const auto type = j.at("type").get<std::string>();
			return deserialization_table().at(type)(j, game_object, component_map);
		}


		Component::Deserializer::Deserializer(
			std::string class_name,
			std::function<Component*(const Io::json&, Game_object&, const Component_type_map&)> factory)
		{
			deserialization_table().emplace(class_name, factory);
		}


		std::unordered_map<std::string, std::function<Component*(const Io::json&, Game_object&, const Component_type_map&)>>&
			Component::
			deserialization_table()
		{
			static auto table = std::unordered_map<std::string, std::function<Component*(
													   const Io::json&,
													   Game_object&,
													   const Component_type_map&)>>{};
			return table;
		}


		Component_loader::Component_loader(Graphics::Render_grid& render_grid)
			: render_grid_{&render_grid} { }


		void Component_loader::register_component(Graphics_component& comp) const
		{
			render_grid_->register_component(comp);
		}


		Component& Component_type_map::get_component(const std::string& type) const
		{
			return *map_.at(type);
		}


		bool Component_type_map::contains(const std::string& type) const
		{
			return map_.find(type) != map_.end();
		}


		void Component_type_map::add_component(Component& component)
		{
			map_.insert_or_assign(component.component_type(), &component);
		}


		void Component_type_map::clear()
		{
			map_.clear();
		}


		Component* Component_type_map::find_component(const std::string& type) const
		{
			const auto iter = map_.find(type);
			return iter == map_.end() ? nullptr : iter->second;
		}
	}
}

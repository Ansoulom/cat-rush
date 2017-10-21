#include "Movement_component.h"
#include "Game_object.h"
#include "World.h"
#include "Component.h"


namespace Game
{
	namespace Objects
	{
		Movement_component::Movement_component(Game_object& game_object) : Component{game_object} { }


		void Movement_component::update(Timer::Seconds time_passed)
		{
			// TODO: Fix for floating point comparisons
			// TODO: Maybe merge x and y for efficiency and unified behaviour, requires new collision formulas though
			if(velocity_.get_x() != 0.0)
			{
				const auto movement = velocity_.get_x() * time_passed.count();
				const auto old_pos = game_object().position();
				game_object().position().x() += movement;
				game_object().send(Events::Position_changed{old_pos});
				game_object().send(Events::Object_moved{Axis::x, old_pos});
			}
			if(velocity_.get_y() != 0.0)
			{
				const auto movement = velocity_.get_y() * time_passed.count();
				const auto old_pos = game_object().position();
				game_object().position().y() += movement;
				game_object().send(Events::Position_changed{old_pos});
				game_object().send(Events::Object_moved{Axis::y, old_pos});
			}
		}


		void Movement_component::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		Geometry::Vector<double>& Movement_component::velocity()
		{
			return velocity_;
		}


		Geometry::Vector<double> Movement_component::velocity() const
		{
			return velocity_;
		}


		Io::json Movement_component::to_json() const
		{
			return {{"type", type()}};
		}


		std::string Movement_component::component_type() const
		{
			return type();
		}


		std::string Movement_component::type()
		{
			return "Movement_component";
		}


		void Movement_component::handle_event(const Events::Collided& message)
		{
			if(message.layer != "solid") return; 
			switch(message.axis)
			{
				case Axis::x:
					velocity_.set_x(0.0);
					break;
				case Axis::y:
					velocity_.set_y(0.0);
					break;
			}
		}


		Movement_component* Movement_component::from_json(
			const Io::json& json,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			return new Movement_component{game_object};
		}


		const Component::Deserializer Movement_component::add_to_map{type(), from_json};
	}
}

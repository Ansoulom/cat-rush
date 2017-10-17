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
				const auto movement = Geometry::Vector<double>{velocity_.get_x() * time_passed.count(), 0.0};
				const auto old_pos = game_object().position();
				game_object().position() += movement;
				game_object().send(Events::Position_changed{old_pos});
				game_object().send(Events::Object_moved{old_pos});
			}
			if(velocity_.get_y() != 0.0)
			{
				const auto movement = Geometry::Vector<double>{0.0, velocity_.get_y() * time_passed.count()};
				const auto old_pos = game_object().position();
				game_object().position() += movement;
				game_object().send(Events::Position_changed{old_pos});
				game_object().send(Events::Object_moved{old_pos});
			}

			/*if(velocity.get_magnitude() > 0.0)
			{
				const auto movement = velocity * time_passed.count();
				const auto old_pos = game_object->get_position();
				game_object->set_position(game_object->get_position() + movement);
				game_object->send(Events::Position_changed{ old_pos });
				game_object->send(Events::Object_moved{ old_pos });
			}*/
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

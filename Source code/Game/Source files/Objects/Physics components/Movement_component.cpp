#include "Movement_component.h"
#include "Game_object.h"
#include "World.h"


namespace Game
{
	namespace Objects
	{
		Movement_component::Movement_component(Game_object& game_object, double speed) : Component{game_object},
																									 max_speed_{speed} { }


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


		void Movement_component::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		void Movement_component::set_x_velocity(double velocity)
		{
			this->velocity_.set_x(velocity);
		}


		void Movement_component::set_y_velocity(double velocity)
		{
			this->velocity_.set_y(velocity);
		}


		double Movement_component::get_speed() const
		{
			return max_speed_;
		}


		IO::json Movement_component::to_json() const
		{
			return {{"type", "Movement_component"}, {"max_speed", max_speed_}}; 
		}


		void Movement_component::handle_event(const Events::Change_velocity_normalized& message)
		{
			assert(message.normalized_velocity.get_magnitude() <= 1.0);
			velocity_ = message.normalized_velocity * max_speed_;
		}


		Movement_component* Movement_component::from_json(const IO::json& json, Game_object& game_object)
		{
			const auto max_speed = json.at("max_speed").get<double>();
			return new Movement_component{game_object, max_speed};
		}


		const Component::Deserializer Movement_component::add_to_map{ "Movement_component", from_json };
	}
}

#include "Collider_component.h"
#include "Shapes.h"
#include "Communication.h"
#include "Collision_system.h"
#include "Game_object.h"
#include "Camera.h"
#include "World.h"


namespace Game
{
	namespace Objects
	{
		Collider_component::Collider_component(
			Game_object& game_object,
			std::unique_ptr<Geometry::Shape<double>> shape,
			std::vector<std::string> layers,
			std::vector<std::string> check_layers)
			: Component{game_object},
			  shape_{move(shape)},
			  layers_{layers},
			  check_layers_{check_layers} { }


		void Collider_component::update(Timer::Seconds time_passed) { }


		void Collider_component::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		const std::vector<std::string>& Collider_component::get_layers() const
		{
			return layers_;
		}


		const std::vector<std::string>& Collider_component::get_check_layers() const
		{
			return check_layers_;
		}


		Collider_component* Collider_component::from_json(
			const Io::json& j,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			const auto component = new Collider_component{
				game_object,
				std::unique_ptr<Geometry::Shape<double>>{Geometry::Shape<double>::from_json(j.at("shape"))},
				j.at("layers").get<std::vector<std::string>>(),
				j.at("check_layers").get<std::vector<std::string>>()
			};
			game_object.world().collision_system().register_component(*component);
			return component;
		}


		Io::json Collider_component::to_json() const
		{
			return {{"type", type()}, {"shape", shape_->to_json()}};
		}


		std::string Collider_component::component_type() const
		{
			return type();
		}


		std::string Collider_component::type()
		{
			return "Collider_component";
		}


		void Collider_component::handle_event(const Events::Object_moved& message)
		{
			auto collisions = game_object().world().collision_system().get_collisions(*this);

			auto old_pos = game_object().position();
			auto direction = old_pos - message.start_position;

			direction.set_x(abs(direction.get_x())); // TODO: FIX EXTREMELY HACKY SOLUTION
			direction.set_y(abs(direction.get_y()));

			direction.set_magnitude(1.0);
			for(auto& col_pair : collisions)
			{
				if(col_pair.second == "solid")
				{
					auto diff = Physics::intersection(*this, *col_pair.first, direction);
					auto new_pos = game_object().position() - diff;
					if(abs((new_pos - message.start_position).get_magnitude()) < abs(
						(game_object().position() - message.start_position).get_magnitude()))
					{
						game_object().position() = new_pos;
					}
				}
			}
			if(game_object().position() != old_pos)
			{
				game_object().send(Events::Position_changed{old_pos});
			}
		}


		void Collider_component::handle_event(const Events::Position_changed& message)
		{
			game_object().world().collision_system().update_grid_position(*this, message.start_position);
		}


		const Component::Deserializer Collider_component::add_to_map{type(), from_json};
	}


	namespace Physics
	{
		bool intersects(const Objects::Collider_component& first, const Objects::Collider_component& second)
		{
			auto first_shape = std::unique_ptr<Geometry::Shape<double>>{ first.shape_->clone() };
			first_shape->position() += first.game_object().position();
			auto second_shape = std::unique_ptr<Geometry::Shape<double>>{ second.shape_->clone() };
			second_shape->position() += second.game_object().position();
			return intersects(*first_shape, *second_shape);
		}


		Geometry::Vector<double> intersection(
			const Objects::Collider_component& first,
			const Objects::Collider_component& second,
			Geometry::Vector<double> direction)
		{
			//assert(direction.get_magnitude() == 1.0); // Floating point comparison
			auto first_shape = std::unique_ptr<Geometry::Shape<double>>{ first.shape_->clone() };
			first_shape->position() += first.game_object().position();
			auto second_shape = std::unique_ptr<Geometry::Shape<double>>{ second.shape_->clone() };
			second_shape->position() += second.game_object().position();
			return intersection(*first_shape, *second_shape, direction);
		}
	}
}

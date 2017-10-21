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


		void Collider_component::set_shape(const Geometry::Shape<double>& shape)
		{
			shape_ = std::unique_ptr<Geometry::Shape<double>>{shape.clone()};
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

			const auto old_pos = game_object().position();
			const auto direction = static_cast<Direction>(Math::sign(
				message.axis == Axis::x ?
					old_pos.get_x() - message.start_position.get_x() :
					old_pos.get_y() - message.start_position.get_y()));

			for(auto& col_pair : collisions)
			{
				if(col_pair.second == "solid")
				{
					switch(message.axis)
					{
						case Axis::x:
						{
							handle_x_collision(
								message.start_position.get_x(),
								*col_pair.first);
							break;
						}
						case Axis::y:
						{
							handle_y_collision(
								message.start_position.get_y(),
								*col_pair.first);
							break;
						}
					}
				}
				game_object().send(Events::Collided{message.axis, direction, *this, *col_pair.first, std::string{col_pair.second}});
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


		void Collider_component::handle_event(const Events::Direction_changed& message) { }


		void Collider_component::handle_x_collision(double start_x, Collider_component& other)
		{
			const auto diff = Physics::x_intersection(*this, other);
			const auto new_x = game_object().position().get_x() - diff;
			if(abs(new_x - start_x) < abs(
				game_object().position().get_x() - start_x))
			{
				game_object().position().set_x(new_x);
			}
		}


		void Collider_component::handle_y_collision(double start_y, Collider_component& other)
		{
			const auto diff = Physics::y_intersection(*this, other);
			const auto new_y = game_object().position().get_y() - diff;
			if(abs(new_y - start_y) < abs(
				game_object().position().get_y() - start_y))
			{
				game_object().position().set_y(new_y);
			}
		}


		const Component::Deserializer Collider_component::add_to_map{type(), from_json};
	}


	namespace Physics
	{
		std::unique_ptr<Geometry::Shape<double>> shape_from_collider(const Objects::Collider_component& collider)
		{
			auto shape = std::unique_ptr<Geometry::Shape<double>>{collider.shape_->clone()};
			shape->position() += collider.game_object().position();
			return move(shape);
		}


		bool intersects(const Objects::Collider_component& first, const Objects::Collider_component& second)
		{
			const auto first_shape = shape_from_collider(first);
			const auto second_shape = shape_from_collider(second);
			return intersects(*first_shape, *second_shape);
		}


		Geometry::Vector<double> intersection(
			const Objects::Collider_component& first,
			const Objects::Collider_component& second,
			Geometry::Vector<double> direction)
		{
			const auto first_shape = shape_from_collider(first);
			const auto second_shape = shape_from_collider(second);
			return intersection(*first_shape, *second_shape, direction);
		}


		double x_intersection(
			const Objects::Collider_component& first,
			const Objects::Collider_component& second)
		{
			const auto first_shape = shape_from_collider(first);
			const auto second_shape = shape_from_collider(second);
			return x_intersection(*first_shape, *second_shape);
		}


		double y_intersection(
			const Objects::Collider_component& first,
			const Objects::Collider_component& second)
		{
			const auto first_shape = shape_from_collider(first);
			const auto second_shape = shape_from_collider(second);
			return y_intersection(*first_shape, *second_shape);
		}
	}
}

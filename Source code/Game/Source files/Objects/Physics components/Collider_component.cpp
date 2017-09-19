#include "Collider_component.h"
#include "Shapes.h"
#include "Communication.h"
#include "Collision_system.h"
#include "Game_object.h"
#include <Camera.h>
#include "Logger.h"


namespace Game
{
	namespace Objects
	{
		Collider_component::Collider_component(Game_object* container, std::unique_ptr<Geometry::Shape<double>> shape, std::vector<std::string> layers, std::vector<std::string> check_layers, Physics::Collision_system& collision_system) : Component{ container }, shape{ move(shape) }, layers{ layers }, check_layers{check_layers}, collision_system { collision_system } // TODO: Look up forwarding
		{
		}


		void Collider_component::update(Timer::Seconds time_passed)
		{
		}


		void Collider_component::receive(const Events::Message& message)
		{
			std::visit([this](const auto& msg){handle_event(msg);}, message);
		}


		void Collider_component::set_position(Geometry::Vector<double> position)
		{
			shape->set_position(position);
		}

		const std::vector<std::string>& Collider_component::get_layers() const
		{
			return layers;
		}

		const std::vector<std::string>& Collider_component::get_check_layers() const
		{
			return check_layers;
		}


		Collider_component* Collider_component::from_json(const IO::json& j, Game_object* game_object, const Component_loader& loader)
		{
			auto component = new Collider_component{game_object, std::unique_ptr<Geometry::Shape<double>>{Geometry::Shape<double>::from_json(j.at("shape"))}, j.at("layers").get<std::vector<std::string>>(), j.at("check_layers").get<std::vector<std::string>>(), loader.physics_loader().collision_system};
			loader.register_component(*component);
			return component;
		}


		IO::json Collider_component::to_json() const
		{
			return { {"type", "Collider_component"}, {"shape", shape->to_json()} };
		}

		
		void Collider_component::handle_event(const Events::Object_moved& message)
		{
			auto collisions = collision_system.get_collisions(*this);

			auto old_pos = game_object->get_position();
			auto direction = old_pos - message.start_position;

			direction.set_x(abs(direction.get_x())); // TODO: FIX EXTREMELY HACKY SOLUTION
			direction.set_y(abs(direction.get_y()));

			direction.set_magnitude(1.0);
			for (auto& col_pair : collisions)
			{
				if (col_pair.second == "solid")
				{
					auto diff = Physics::intersection(*this, *col_pair.first, direction);
					auto new_pos = game_object->get_position() - diff; // Plus or minus?
					if (abs((new_pos - message.start_position).get_magnitude()) < abs((game_object->get_position() - message.start_position).get_magnitude()))
					{
						game_object->set_position(new_pos);
					}
				}
			}
			if (game_object->get_position() != old_pos)
			{
				game_object->send(Events::Position_changed{ old_pos });
			}
		}


		void Collider_component::handle_event(const Events::Position_changed& message)
		{
			set_position(game_object->get_position());
			collision_system.update_grid_position(*this, message.start_position);
		}
	}

	namespace Physics
	{
		bool intersects(const Objects::Collider_component& first, const Objects::Collider_component& second)
		{
			return intersects(*first.shape, *second.shape);
		}


		Geometry::Vector<double> intersection(const Objects::Collider_component & first, const Objects::Collider_component & second, Geometry::Vector<double> direction)
		{
			//assert(direction.get_magnitude() == 1.0); // Floating point comparison
			return intersection(*first.shape, *second.shape, direction);
		}
	}
}

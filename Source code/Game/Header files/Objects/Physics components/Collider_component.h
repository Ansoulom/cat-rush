#pragma once

#include <json.hpp>
#include "Component.h"


namespace Game
{
	namespace Geometry
	{
		template<typename T>
		class Shape;
	}


	namespace Objects
	{
		class Collider_component;
	}


	namespace Physics
	{
		class Collision_system;

		std::unique_ptr<Geometry::Shape<double>> shape_from_collider(const Objects::Collider_component& collider);
		bool intersects(const Objects::Collider_component& first, const Objects::Collider_component& second);
		Geometry::Vector<double> intersection(
			const Objects::Collider_component& first,
			const Objects::Collider_component& second,
			Geometry::Vector<double> direction);
		double x_intersection(const Objects::Collider_component& first, const Objects::Collider_component& second);
		double y_intersection(const Objects::Collider_component& first, const Objects::Collider_component& second);
	}


	namespace Objects
	{
		class Collider_component : public Component
		{
		public:
			Collider_component(
				Game_object& game_object,
				std::unique_ptr<Geometry::Shape<double>> shape,
				std::vector<std::string> layers,
				std::vector<std::string> check_layers);

			~Collider_component() override;

			// Serialization
			static Collider_component* from_json(const Io::json& j, Game_object& game_object,
				const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

			// Overridden functions
			void update(Timer::Seconds time_passed) override;
			void receive(const Events::Message& message) override;

			// Collider functions
			const std::vector<std::string>& get_layers() const;
			const std::vector<std::string>& get_check_layers() const;
			void set_shape(const Geometry::Shape<double>& shape);

		private:
			template<typename T>
			void handle_event(const T& message) {}


			void handle_event(const Events::Object_moved& message);
			void handle_event(const Events::Position_changed& message);
			void handle_event(const Events::Direction_changed& message);

			void handle_x_collision(double start_x, Collider_component& other);
			void handle_y_collision(double start_y, Collider_component& other);

			std::unique_ptr<Geometry::Shape<double>> shape_;
			std::vector<std::string> layers_, check_layers_;

			// Required for serialization
			static const Deserializer add_to_map;

			friend std::unique_ptr<Geometry::Shape<double>> Physics::shape_from_collider(const Collider_component& collider);
		};
	}
}

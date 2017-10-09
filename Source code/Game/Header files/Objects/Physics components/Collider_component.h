#pragma once

#include <json.hpp>
#include "Component.h"
#include "Component.h"


namespace Game
{
	namespace Geometry
	{
		template<typename type>
		class Shape;
	}


	namespace Objects
	{
		class Collider_component;
	}


	namespace Physics
	{
		class Collision_system;

		bool intersects(const Objects::Collider_component& first, const Objects::Collider_component& second);
		Geometry::Vector<double> intersection(
			const Objects::Collider_component& first,
			const Objects::Collider_component& second,
			Geometry::Vector<double> direction);
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

			void update(Timer::Seconds time_passed) override;
			void receive(const Events::Message& message) override;

			void set_position(Geometry::Vector<double> position);
			const std::vector<std::string>& get_layers() const;
			const std::vector<std::string>& get_check_layers() const;

			static Collider_component* from_json(const IO::json& j, Game_object& game_object);
			IO::json to_json() const override;

		private:
			template<typename T>
			void handle_event(const T& message) {}


			void handle_event(const Events::Object_moved& message);
			void handle_event(const Events::Position_changed& message);

			std::unique_ptr<Geometry::Shape<double>> shape_;
			std::vector<std::string> layers_, check_layers_;

			static const Deserializer add_to_map;

			friend bool Physics::intersects(const Collider_component& first, const Collider_component& second);
			friend Geometry::Vector<double> Physics::intersection(
				const Collider_component& first,
				const Collider_component& second,
				Geometry::Vector<double> direction);
		};
	}
}

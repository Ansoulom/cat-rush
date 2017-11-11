#pragma once

#include <vector>
#include <unordered_map>
#include "Vector.h"
#include "Shapes.h"


namespace Game
{
	namespace Objects
	{
		class Collider_component;
		class Game_object;
	}

	namespace Physics
	{
		class Collision_system
		{
		public:
			std::vector<std::pair<Objects::Collider_component*, std::string_view>> get_collisions(const Objects::Collider_component& collider) const;
			std::vector<Objects::Collider_component*> get_collisions(const Geometry::Shape<double>& collider, const std::string& layer) const;
			void register_component(Objects::Collider_component& comp);
			void remove_component(Objects::Collider_component& comp);
			void update_grid_position(Objects::Collider_component& comp, Geometry::Vector<double> old_position);

		private:
			void register_component(Objects::Collider_component& comp, Geometry::Vector<int> grid_cell_pos);
			void remove_component(Objects::Collider_component& comp, Geometry::Vector<int> grid_cell_pos);
			static Geometry::Vector<int> get_grid_cell_position(Geometry::Vector<double> object_position);
			
			std::unordered_map<Geometry::Vector<int>, std::unordered_map<std::string, std::list<Objects::Collider_component*>>> grid_; // Consider using some kind of IDs instead of pointers

			static const double grid_cell_size_;
		};
	}
}

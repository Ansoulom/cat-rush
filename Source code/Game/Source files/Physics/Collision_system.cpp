#include "Collision_system.h"
#include "Collider_component.h"


namespace Game
{
	namespace Physics
	{
		std::vector<std::pair<Objects::Collider_component*, std::string_view>> Collision_system::get_collisions(
			const Objects::Collider_component& collider) const
		{
			auto colliders = std::vector<std::pair<Objects::Collider_component*, std::string_view>>{};

			auto start = get_grid_cell_position(collider.get_position()) - Geometry::Vector<int>{1, 1};
			auto end = get_grid_cell_position(collider.get_position()) + Geometry::Vector<int>{1, 1};
			for(auto x = start.get_x(); x <= end.get_x(); ++x)
			{
				for(auto y = start.get_y(); y <= end.get_y(); ++y)
				{
					auto cell_pos = Geometry::Vector<int>{x, y};
					auto iter = grid.find(cell_pos);
					if(iter != grid.end())
					{
						auto& layers = collider.get_check_layers();
						for(auto& layer : layers)
						{
							auto layer_iter = iter->second.find(layer);
							if(layer_iter != iter->second.end())
							{
								for(auto component : layer_iter->second)
								{
									if(&collider == component) continue;
									if(intersects(collider, *component))
									{
										colliders.emplace_back(component, layer);
									}
								}
							}
						}
					}
				}
			}

			return colliders;
		}


		void Collision_system::register_component(Objects::Collider_component& comp)
		{
			register_component(comp, get_grid_cell_position(comp.get_position()));
		}


		void Collision_system::remove_component(Objects::Collider_component& comp)
		{
			remove_component(comp, get_grid_cell_position(comp.get_position()));
		}


		void Collision_system::update_grid_position(Objects::Collider_component& comp, Geometry::Vector<double> old_position)
		{
			auto old_cell_pos = get_grid_cell_position(old_position);
			auto cell_pos = get_grid_cell_position(comp.get_position());

			if(old_cell_pos != cell_pos)
			{
				remove_component(comp, old_cell_pos);
				register_component(comp, cell_pos);
			}
		}


		void Collision_system::register_component(Objects::Collider_component& comp, Geometry::Vector<int> grid_cell_pos)
		{
			for(auto& layer : comp.get_layers())
			{
				grid[grid_cell_pos][layer].push_back(&comp);
			}
		}


		void Collision_system::remove_component(Objects::Collider_component& comp, Geometry::Vector<int> grid_cell_pos)
		{
			auto iter = grid.find(grid_cell_pos);
			if(iter != grid.end())
			{
				auto& layers = comp.get_layers();
				for(auto& layer : layers)
				{
					auto layer_iter = iter->second.find(layer);
					if(layer_iter != iter->second.end())
					{
						layer_iter->second.erase(remove(layer_iter->second.begin(), layer_iter->second.end(), &comp));
					}
				}
			}
		}


		Geometry::Vector<int> Collision_system::get_grid_cell_position(Geometry::Vector<double> object_position)
		{
			auto cell_x = static_cast<int>(object_position.get_x() / grid_cell_size);
			auto cell_y = static_cast<int>(object_position.get_y() / grid_cell_size);
			return {cell_x, cell_y};
		}


		const double Collision_system::grid_cell_size{10.0}; // VERY TEMPORARY VALUE
	}
}

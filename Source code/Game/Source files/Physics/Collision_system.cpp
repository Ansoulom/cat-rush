#include "Collision_system.h"
#include "Collider_component.h"
#include "Game_object.h"


namespace Game
{
	namespace Physics
	{
		std::vector<std::pair<Objects::Collider_component*, std::string_view>> Collision_system::get_collisions(
			const Objects::Collider_component& collider) const
		{
			auto colliders = std::vector<std::pair<Objects::Collider_component*, std::string_view>>{};

			auto start = get_grid_cell_position(collider.game_object().position()) - Geometry::Vector<int>{1, 1};
			auto end = get_grid_cell_position(collider.game_object().position()) + Geometry::Vector<int>{1, 1};
			for(auto x = start.get_x(); x <= end.get_x(); ++x)
			{
				for(auto y = start.get_y(); y <= end.get_y(); ++y)
				{
					const auto cell_pos = Geometry::Vector<int>{x, y};
					const auto iter = grid_.find(cell_pos);
					if(iter != grid_.end())
					{
						for(auto& layer : collider.get_check_layers())
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


		std::vector<Objects::Collider_component*> Collision_system::get_collisions(
			const Geometry::Shape<double>& collider,
			const std::string& layer) const
		{
			auto colliders = std::vector<Objects::Collider_component*>{};

			auto start = get_grid_cell_position(collider.position()) - Geometry::Vector<int>{1, 1};
			auto end = get_grid_cell_position(collider.position()) + Geometry::Vector<int>{1, 1};
			for(auto x = start.get_x(); x <= end.get_x(); ++x)
			{
				for(auto y = start.get_y(); y <= end.get_y(); ++y)
				{
					const auto cell_pos = Geometry::Vector<int>{x, y};
					const auto iter = grid_.find(cell_pos);
					if(iter != grid_.end())
					{
						auto layer_iter = iter->second.find(layer);
						if(layer_iter != iter->second.end())
						{
							for(auto component : layer_iter->second)
							{
								const auto shape = shape_from_collider(*component);
								if(intersects(collider, *shape))
								{
									colliders.emplace_back(component);
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
			register_component(comp, get_grid_cell_position(comp.game_object().position()));
		}


		void Collision_system::remove_component(Objects::Collider_component& comp)
		{
			remove_component(comp, get_grid_cell_position(comp.game_object().position()));
		}


		void Collision_system::update_grid_position(Objects::Collider_component& comp, Geometry::Vector<double> old_position)
		{
			auto old_cell_pos = get_grid_cell_position(old_position);
			auto cell_pos = get_grid_cell_position(comp.game_object().position());

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
				grid_[grid_cell_pos][layer].push_back(&comp);
			}
		}


		void Collision_system::remove_component(Objects::Collider_component& comp, Geometry::Vector<int> grid_cell_pos)
		{
			auto iter = grid_.find(grid_cell_pos);
			if(iter != grid_.end())
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
			auto cell_x = static_cast<int>(object_position.get_x() / grid_cell_size_);
			auto cell_y = static_cast<int>(object_position.get_y() / grid_cell_size_);
			return {cell_x, cell_y};
		}


		const double Collision_system::grid_cell_size_{10.0}; // VERY TEMPORARY VALUE
	}
}

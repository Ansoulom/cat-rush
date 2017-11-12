#include "HUD.h"
#include "Renderer.h"
#include "World.h"


namespace Game
{
	namespace Gui
	{
		Health_bar::Health_bar(Geometry::Rectangle<double> bounds, bool flipped)
			: bounds_{bounds}, flipped_{flipped},
			  health_changed_receiver_{std::bind(&Health_bar::set_health, this, std::placeholders::_1)} {}


		void Health_bar::set_max_health(int max_health)
		{
			assert(max_health > 0);
			max_health_ = max_health;
		}


		void Health_bar::set_health(int health)
		{
			assert(health >= 0);
			assert(health <= max_health_);
			health_ = health;
		}


		void Health_bar::connect_with_object(Objects::Combat_component& object)
		{
			set_max_health(object.max_health());
			set_health(object.current_health());
			object.add_health_changed_receiver(health_changed_receiver_);
		}


		std::vector<Graphics::Render_instance> Health_bar::get_render_instances(
			const Resources::Texture_manager& tm,
			const Graphics::Render_settings& render_settings) const
		{
			const auto health_percentage = static_cast<double>(health_) / max_health_;
			const auto& bg_texture = tm.get_texture("health_bar_bg");
			const auto& bar_texture = tm.get_texture("health_bar");
			const auto bg_instance = Graphics::Render_instance{
				bg_texture,
				{
					render_settings.normalized_to_pixels({bounds_.left(), bounds_.top()}),
					render_settings.normalized_to_pixels(bounds_.width()),
					render_settings.normalized_to_pixels(bounds_.height()),
					Geometry::Pivot_point::top_left
				},
				4
			};
			const auto bar_instance = Graphics::Render_instance{
				bar_texture,
				{
					render_settings.normalized_to_pixels({bounds_.left(), bounds_.top()}),
					render_settings.normalized_to_pixels(bounds_.width() * health_percentage),
					render_settings.normalized_to_pixels(bounds_.height()),
					Geometry::Pivot_point::top_left
				},
				5,
				Geometry::Rectangle<int>{
					{0, 0},
					static_cast<int>(round(bar_texture.width() * health_percentage)),
					bar_texture.height(),
					Geometry::Pivot_point::top_left
				}
			};

			return {bg_instance, bar_instance};
		}


		Hud::Hud(Graphics::Aspect_ratio aspect_ratio)
			: aspect_ratio_{aspect_ratio},
			  player_bar_{{{0.5, 0.2}, 0.4, 0.05, Geometry::Pivot_point::center_right}, false},
			  boss_bar_{{{0.8, 0.2}, 0.4, 0.05, Geometry::Pivot_point::center_left}, true} { }


		void Hud::connect_with_world(Core::World& world)
		{
			if(auto player = world.find_object_by_name("player"))
			{
				if(const auto player_combat = player->find_component<Objects::Combat_component>())
					player_bar_.connect_with_object(*player_combat);
			}
			if(auto boss = world.find_object_by_name("boss"))
			{
				if(const auto boss_combat = boss->find_component<Objects::Combat_component>())
					boss_bar_.connect_with_object(*boss_combat);
			}
		}


		std::vector<Graphics::Render_instance> Hud::get_render_instances(
			const Resources::Texture_manager& tm,
			const Graphics::Render_settings& render_settings)
		{
			auto instances = player_bar_.get_render_instances(tm, render_settings);
			auto boss_bar_instances = boss_bar_.get_render_instances(tm, render_settings);
			instances.insert(instances.end(), boss_bar_instances.begin(), boss_bar_instances.end());

			return instances;
		}
	}
}

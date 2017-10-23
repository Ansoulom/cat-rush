#include "HUD.h"
#include "Renderer.h"


namespace Game
{
	namespace Gui
	{
		Health_bar::Health_bar(Geometry::Rectangle<double> bounds, bool flipped)
			: max_health_{100}, health_{max_health_}, bounds_{bounds}, flipped_{flipped} {}


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
					render_settings.normalized_to_pixels(bounds_.position()),
					render_settings.normalized_to_pixels(bounds_.width()),
					render_settings.normalized_to_pixels(bounds_.height())
				},
				4
			};
			const auto bar_instance = Graphics::Render_instance{
				bar_texture,
				{
					render_settings.normalized_to_pixels(bounds_.position()),
					static_cast<int>(ceil(render_settings.normalized_to_pixels(bounds_.width()) * health_percentage)),
					render_settings.normalized_to_pixels(bounds_.height())
				},
				5,
				Geometry::Rectangle<int>{
					{0, 0},
					static_cast<int>(ceil(bar_texture.width() * health_percentage)),
					bar_texture.height(),
					Geometry::Pivot_point::top_left
				}
			};

			return {bg_instance, bar_instance};
		}
	}
}

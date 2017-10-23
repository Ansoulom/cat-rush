#include "HUD.h"


namespace Game
{
	namespace Gui
	{
		Health_bar::Health_bar(Geometry::Rectangle<double> bounds, bool flipped)
			: max_health_{100}, health_{max_health_}, bounds_{bounds}, flipped_{flipped} {}


		std::vector<Graphics::Render_instance> Health_bar::get_render_instances(const Resources::Texture_manager& tm) const
		{
			auto health_percentage = static_cast<double>(health_) / max_health_;
			auto bg_texture = tm.get_texture("health_bar_bg");
			auto bar_texture = tm.get_texture("health_bar");
			auto bg_instance = Graphics::Render_instance{*bg_texture, bounds_.position(), 4};
			auto bar_instance = Graphics::Render_instance{
				*bar_texture,
				bounds_.position(),
				5,
				Geometry::Rectangle<int>{
					{0, 0},
					static_cast<int>(ceil(bar_texture->get_width() * health_percentage)),
					bar_texture->get_height(),
					Geometry::Pivot_point::top_left
				},
				{},
				{},
				{},
				{},
				{health_percentage, bounds_.height()}
			};

			return {bg_instance, bar_instance};
		}
	}
}

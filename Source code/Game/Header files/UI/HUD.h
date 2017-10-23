#pragma once

#include "Shapes.h"
#include "Render_instance.h"
#include "Resource_managers.h"
#include "Renderer.h"


namespace Game
{
	namespace Graphics
	{
		struct Render_settings;
	}
}


namespace Game
{
	namespace Gui
	{
		class Health_bar
		{
		public:
			Health_bar(Geometry::Rectangle<double> bounds, bool flipped);

			std::vector<Graphics::Render_instance> get_render_instances(
				const Resources::Texture_manager& tm,
				const Graphics::Render_settings& render_settings) const;

		private:
			int max_health_;
			int health_;
			Geometry::Rectangle<double> bounds_;
			bool flipped_;
		};


		class Hud
		{
		public:
			explicit Hud(Graphics::Aspect_ratio aspect_ratio);

			std::vector<Graphics::Render_instance> get_render_instances(
				const Resources::Texture_manager& tm,
				const Graphics::Render_settings& render_settings);

		private:
			Graphics::Aspect_ratio aspect_ratio_;
			Health_bar player_bar_, boss_bar_;
		};
	}
}

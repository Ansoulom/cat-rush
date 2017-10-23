#pragma once

#include "Shapes.h"
#include "Render_instance.h"
#include "Resource_managers.h"


namespace Game
{
	namespace Gui
	{
		class Health_bar
		{
		public:
			Health_bar(Geometry::Rectangle<double> bounds, bool flipped);

			std::vector<Graphics::Render_instance> get_render_instances(const Resources::Texture_manager& tm) const;

		private:
			int max_health_;
			int health_;
			Geometry::Rectangle<double> bounds_;
			bool flipped_;
		};
	}
}

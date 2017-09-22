#pragma once

#include "Texture.h"
#include "Shapes.h"


namespace Game
{
	namespace Graphics
	{
		class Render_instance
		{
		public:
			Render_instance(Texture* texture, Geometry::Rectangle<int> destination, const Geometry::Rectangle<int>* source = {}, double angle = 0.0, const Geometry::Vector<int>* pivot_point = {}, bool flipped_x = false, bool flipped_y = false); // TODO: Use std::optional instead of nullptr when C++17 is enabled

			Render_instance(const Render_instance& other);
			Render_instance(Render_instance&& other) noexcept;

			Render_instance& operator=(const Render_instance& other);
			Render_instance& operator=(Render_instance&& other) noexcept;

		private:
			Texture* texture_;
			Geometry::Rectangle<int> destination_, source_;
			double angle_;
			Geometry::Vector<int> pivot_;
			bool flipped_x_, flipped_y_;

			friend class Renderer;
		};
	}
}

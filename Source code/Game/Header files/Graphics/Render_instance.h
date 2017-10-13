#pragma once

#include "Texture.h"
#include "Shapes.h"
#include <optional>


namespace Game
{
	namespace Graphics
	{
		class Render_instance
		{
		public:
			Render_instance(Texture* texture, Geometry::Vector<double> destination, const std::optional<Geometry::Rectangle<int>> source = {}, double angle = {}, const std::optional<Geometry::Vector<double>> pivot_point = {}, bool flipped_x = false, bool flipped_y = false, Geometry::Vector<double> scaling = {1.0, 1.0});

			Render_instance(const Render_instance& other);
			Render_instance(Render_instance&& other) noexcept;

			Render_instance& operator=(const Render_instance& other);
			Render_instance& operator=(Render_instance&& other) noexcept;

		private:
			Texture* texture_;
			Geometry::Rectangle<int> source_;
			Geometry::Vector<double> destination_;
			Geometry::Vector<double> scaling_;
			double angle_;
			Geometry::Vector<double> pivot_;
			bool flipped_x_, flipped_y_;

			friend class Renderer;
		};
	}
}

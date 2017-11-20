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
			Render_instance(
				const Texture& texture,
				Geometry::Rectangle<int> destination,
				int layer,
				std::optional<Geometry::Rectangle<int>> source = {},
				bool flipped_x = false,
				bool flipped_y = false,
				double angle = {},
				std::optional<Geometry::Vector<int>> pivot_point = {});

			Render_instance(const Render_instance& other);
			Render_instance(Render_instance&& other) noexcept;

			Render_instance& operator=(const Render_instance& other);
			Render_instance& operator=(Render_instance&& other) noexcept;

		private:
			const Texture* texture_;
			Geometry::Rectangle<int> destination_;
			int layer_;
			Geometry::Rectangle<int> source_;
			bool flipped_x_, flipped_y_;
			double angle_;
			Geometry::Vector<int> pivot_;

			friend class Renderer;
			friend class Render_grid;
		};
	}
}

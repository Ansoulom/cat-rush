#include "Render_instance.h"


namespace Game
{
	namespace Graphics
	{
		Render_instance::Render_instance(
			const Texture& texture,
			Geometry::Rectangle<int> destination,
			int layer,
			std::optional<Geometry::Rectangle<int>> source,
			bool flipped_x,
			bool flipped_y,
			double angle,
			std::optional<Geometry::Vector<int>> pivot_point)
			: texture_{&texture},
			  destination_{destination},
			  layer_{layer},
			  source_{
				  source ?
					  *source :
					  Geometry::Rectangle<int>{{0, 0}, texture.width(), texture.height(), Geometry::Pivot_point::top_left}
			  },
			  flipped_x_{flipped_x},
			  flipped_y_{flipped_y},
			  angle_{angle},
			  pivot_{pivot_point ? *pivot_point : Geometry::Vector<int>{}} { }


		Render_instance::Render_instance(const Render_instance& other)
			: texture_{other.texture_},
			  destination_{other.destination_},
			  layer_{other.layer_},
			  source_{other.source_},
			  flipped_x_{other.flipped_x_},
			  flipped_y_{other.flipped_y_},
			  angle_{other.angle_},
			  pivot_{other.pivot_} { }


		Render_instance::Render_instance(Render_instance&& other) noexcept
			: texture_{other.texture_},
			  destination_{other.destination_},
			  layer_{other.layer_},
			  source_{other.source_},
			  flipped_x_{other.flipped_x_},
			  flipped_y_{other.flipped_y_},
			  angle_{other.angle_},
			  pivot_{other.pivot_} { }


		Render_instance& Render_instance::operator=(const Render_instance& other)
		{
			texture_ = other.texture_;
			destination_ = other.destination_;
			layer_ = other.layer_;
			source_ = other.source_;
			flipped_x_ = other.flipped_x_;
			flipped_y_ = other.flipped_y_;
			angle_ = other.angle_;
			pivot_ = other.pivot_;

			return *this;
		}


		Render_instance& Render_instance::operator=(Render_instance&& other) noexcept // Is this a bad move function?
		{
			texture_ = other.texture_;
			destination_ = other.destination_;
			layer_ = other.layer_;
			source_ = other.source_;
			flipped_x_ = other.flipped_x_;
			flipped_y_ = other.flipped_y_;
			angle_ = other.angle_;
			pivot_ = other.pivot_;

			return *this;
		}
	}
}

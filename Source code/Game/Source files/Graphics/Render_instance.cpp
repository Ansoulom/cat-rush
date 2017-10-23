#include "Render_instance.h"


namespace Game
{
	namespace Graphics
	{
		Render_instance::Render_instance(
			Texture& texture,
			Geometry::Vector<double> destination,
			int layer,
			const std::optional<Geometry::Rectangle<int>> source,
			double angle,
			const std::optional<Geometry::Vector<double>> pivot_point,
			bool flipped_x,
			bool flipped_y,
			Geometry::Vector<double> scaling)
			: texture_{&texture},
			  source_{
				  source ?
					  *source :
					  Geometry::Rectangle<int>{{0, 0}, texture.get_width(), texture.get_height(), Geometry::Pivot_point::top_left}
			  },
			  destination_{destination},
			  scaling_{scaling},
			  angle_{angle},
			  pivot_{pivot_point ? *pivot_point : Geometry::Vector<double>{}},
			  flipped_x_{flipped_x},
			  flipped_y_{flipped_y},
			  layer_{layer} { }


		Render_instance::Render_instance(const Render_instance& other)
			: texture_{other.texture_},
			  source_{other.source_},
			  destination_{other.destination_},
			  scaling_{other.scaling_},
			  angle_{other.angle_},
			  pivot_{other.pivot_},
			  flipped_x_{other.flipped_x_},
			  flipped_y_{other.flipped_y_},
			  layer_{other.layer_} { }


		Render_instance::Render_instance(Render_instance&& other) noexcept
			: texture_{other.texture_},
			  source_{other.source_},
			  destination_{other.destination_},
			  scaling_{other.scaling_},
			  angle_{other.angle_},
			  pivot_{other.pivot_},
			  flipped_x_{other.flipped_x_},
			  flipped_y_{other.flipped_y_},
			  layer_{other.layer_} { }


		Render_instance& Render_instance::operator=(const Render_instance& other)
		{
			texture_ = other.texture_;
			destination_ = other.destination_;
			source_ = other.source_;
			angle_ = other.angle_;
			pivot_ = other.pivot_;
			flipped_x_ = other.flipped_x_;
			flipped_y_ = other.flipped_y_;
			scaling_ = other.scaling_;
			layer_ = other.layer_;

			return *this;
		}


		Render_instance& Render_instance::operator=(Render_instance&& other) noexcept // Is this a bad move function?
		{
			texture_ = other.texture_;
			destination_ = other.destination_;
			source_ = other.source_;
			angle_ = other.angle_;
			pivot_ = other.pivot_;
			flipped_x_ = other.flipped_x_;
			flipped_y_ = other.flipped_y_;
			scaling_ = other.scaling_;
			layer_ = other.layer_;

			return *this;
		}
	}
}

#include "Render_instance.h"


namespace Game
{
	namespace Graphics
	{
		Render_instance::Render_instance(Texture* texture, Geometry::Rectangle<int> destination, const Geometry::Rectangle<int>* source, double angle, const Geometry::Vector<int>* pivot_point, bool flipped_x, bool flipped_y) : texture{texture}, destination{destination}, source{source ? *source : Geometry::Rectangle<int>{{0, 0}, texture->get_width(), texture->get_height(), Geometry::Pivot_point::top_left}}, angle{angle}, pivot{pivot_point ? *pivot_point : Geometry::Vector<int>{destination.width() / 2, destination.height() / 2}}, flipped_x{flipped_x}, flipped_y{flipped_y}
		{
		}


		Render_instance::Render_instance(const Render_instance& other) : texture{other.texture}, destination{other.destination}, source{other.source}, angle{other.angle}, pivot{other.pivot}, flipped_x{other.flipped_x}, flipped_y{other.flipped_y}
		{
		}


		Render_instance::Render_instance(Render_instance&& other) noexcept : texture{other.texture}, destination{other.destination}, source{other.source}, angle{other.angle}, pivot{other.pivot}, flipped_x{other.flipped_x}, flipped_y{other.flipped_y}
		{
		}


		Render_instance& Render_instance::operator=(const Render_instance& other)
		{
			texture = other.texture;
			destination = other.destination;
			source = other.source;
			angle = other.angle;
			pivot = other.pivot;
			flipped_x = other.flipped_x;
			flipped_y = other.flipped_y;

			return *this;
		}


		Render_instance& Render_instance::operator=(Render_instance&& other) noexcept // Is this a bad move function?
		{
			texture = other.texture;
			destination = other.destination;
			source = other.source;
			angle = other.angle;
			pivot = other.pivot;
			flipped_x = other.flipped_x;
			flipped_y = other.flipped_y;

			return *this;
		}
	}
}

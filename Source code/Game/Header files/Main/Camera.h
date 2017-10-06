#pragma once

#include "Timer.h"
#include "Vector.h"
#include "Shapes.h"
#include "Graphics_component.h"
#include "Renderer.h"


namespace Game
{
	namespace Objects
	{
		class Game_object;
	}


	class Camera
	{
	public:
		explicit Camera(Geometry::Vector<double> position, Graphics::Aspect_ratio aspect_ratio); // Should be the camera's center position.
		explicit Camera(Objects::Game_object* focus, Graphics::Aspect_ratio aspect_ratio); // TODO: Fix this shit
		~Camera();

		void update(Timer::Seconds time_passed);
		Geometry::Vector<double> get_position() const;
		Geometry::Vector<double> get_coordinates_on_screen(Geometry::Vector<double> position_in_world) const;
		Geometry::Rectangle<double> bounding_box() const;
		void set_aspect_ratio(Graphics::Aspect_ratio aspect_ratio);

	private:
		Graphics::Aspect_ratio aspect_ratio_;
		double zoom_; // If zoom_ = 1.0, the screen height will equal 1 world unit. Higher number is more zoomed in.
		Objects::Game_object* focus_; // TODO: Make sure that focus can't be a dangling pointer.
		Geometry::Vector<double> position_;
	};
}

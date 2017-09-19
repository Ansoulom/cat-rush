#pragma once

#include "Timer.h"
#include "Vector.h"
#include "Shapes.h"


namespace Game
{
	namespace Objects
	{
		class Game_object;
	}


	class Camera
	{
	public:
		explicit Camera(Geometry::Vector<double> position); // Should be the camera's center position.
		explicit Camera(Objects::Game_object* focus);
		~Camera();

		void update(Timer::Seconds time_passed);
		Geometry::Vector<double> get_position() const;
		Geometry::Vector<int> get_coordinates_on_screen(Geometry::Vector<double> position_in_world) const;
		Geometry::Rectangle<double> bounding_box() const;

	private:
		Objects::Game_object* focus; // TODO: Make sure that focus can't be a dangling pointer.
		Geometry::Vector<double> position;
	};
}

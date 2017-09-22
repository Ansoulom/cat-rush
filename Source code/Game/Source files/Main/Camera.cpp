#include "Camera.h"
#include "Game_object.h"
#include <Game_core.h>


namespace Game
{
	Camera::Camera(Geometry::Vector<double> position) : focus_{nullptr}, position_{position}
	{
	}


	Camera::Camera(Objects::Game_object* focus) : focus_{focus}
	{
		if(focus)
		{
			position_ = focus->get_position();
		}
	}


	Camera::~Camera()
	{
	}


	void Camera::update(Timer::Seconds)
	{
		//TODO Fixa så att focus inte kan peka mot ett dött objekt.
		if(focus_)
		{
			position_ = focus_->get_position();
		}
	}


	Geometry::Vector<double> Camera::get_position() const
	{
		return position_;
	}


	Geometry::Vector<int> Camera::get_coordinates_on_screen(Geometry::Vector<double> position_in_world) const
	{
		auto x_render = Game_core::convert_meters_to_pixels(position_in_world.get_x() - position_.get_x()) + Game_core::source_width / 2;
		auto y_render = Game_core::convert_meters_to_pixels(position_in_world.get_y() - position_.get_y()) + Game_core::source_height / 2;
		return {x_render, y_render};
	}


	Geometry::Rectangle<double> Camera::bounding_box() const
	{
		auto w = Game_core::convert_pixels_to_meters(Game_core::source_width);
		auto h = Game_core::convert_pixels_to_meters(Game_core::source_height);
		return {position_, w, h};
	}
}

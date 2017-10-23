#include "Camera.h"
#include "Game_object.h"
#include <Game_core.h>


namespace Game
{
	Camera::Camera(Geometry::Vector<double> position, Graphics::Aspect_ratio aspect_ratio)
		: aspect_ratio_{aspect_ratio},
		  zoom_{1.0},
		  focus_{nullptr},
		  position_{position} { }


	Camera::Camera(Objects::Game_object* focus, Graphics::Aspect_ratio aspect_ratio)
		: aspect_ratio_{aspect_ratio},
		  zoom_{1.0},
		  focus_{focus}
	{
		if(focus)
		{
			position_ = focus->position();
		}
	}


	void Camera::update(Timer::Seconds)
	{
		//TODO Fixa så att focus inte kan peka mot ett dött objekt.
		if(focus_)
		{
			position_ = focus_->position();
		}
	}


	Geometry::Vector<double> Camera::position() const
	{
		return position_;
	}


	Geometry::Vector<double> Camera::get_normalized_coordinates_on_screen(Geometry::Vector<double> position_in_world) const
	{
		const auto x_render = (position_in_world.get_x() - position_.get_x() + aspect_ratio_.x_multiplier() / 2) * zoom_;
		const auto y_render = (position_in_world.get_y() - position_.get_y() + aspect_ratio_.y_multiplier() / 2) * zoom_;

		return {x_render, y_render};
	}


	Geometry::Rectangle<double> Camera::bounding_box() const
	{
		const auto w = aspect_ratio_.x_multiplier() / zoom_;
		const auto h = aspect_ratio_.y_multiplier() / zoom_;

		return {position_, w, h};
	}


	void Camera::set_aspect_ratio(Graphics::Aspect_ratio aspect_ratio)
	{
		aspect_ratio_ = aspect_ratio;
	}
}

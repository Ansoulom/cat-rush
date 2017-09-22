#include "Frame_timer.h"

namespace Game
{
	Frame_timer::Frame_timer(): framerate_{0}, frames_{0}, time_passed_{}, previous_time_{std::chrono::high_resolution_clock::now()}
	{
	}

	std::chrono::nanoseconds Frame_timer::update()
	{
		auto current_time = std::chrono::high_resolution_clock::now();
		auto delta_time = current_time - previous_time_;
		previous_time_ = current_time;
		this->delta_time_ = delta_time;

		++frames_;
		this->time_passed_ += delta_time;
		if(this->time_passed_ >= std::chrono::seconds{1})
		{
			this->time_passed_ -= std::chrono::seconds{1};
			framerate_ = frames_;
			frames_ = 0;
		}

		return delta_time;
	}

	std::chrono::nanoseconds Frame_timer::get_delta_time() const
	{
		return delta_time_;
	}

	int Frame_timer::get_framerate() const
	{
		return framerate_;
	}
}

#include "Frame_timer.h"

namespace Game
{
	Frame_timer::Frame_timer(): framerate{0}, frames{0}, time_passed{}, previous_time{std::chrono::high_resolution_clock::now()}
	{
	}

	std::chrono::nanoseconds Frame_timer::update()
	{
		auto current_time = std::chrono::high_resolution_clock::now();
		auto delta_time = current_time - previous_time;
		previous_time = current_time;
		this->delta_time = delta_time;

		++frames;
		this->time_passed += delta_time;
		if(this->time_passed >= std::chrono::seconds{1})
		{
			this->time_passed -= std::chrono::seconds{1};
			framerate = frames;
			frames = 0;
		}

		return delta_time;
	}

	std::chrono::nanoseconds Frame_timer::get_delta_time() const
	{
		return delta_time;
	}

	int Frame_timer::get_framerate() const
	{
		return framerate;
	}
}

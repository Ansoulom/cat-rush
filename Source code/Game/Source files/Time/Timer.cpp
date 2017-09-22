#include "Timer.h"
#include <chrono>

namespace Game
{
	Timer::Timer(Seconds max_time) : time_passed_{0.0}, target_time_{max_time}
	{
	}

	bool Timer::update(Seconds time_passed)
	{
		this->time_passed_ += time_passed;

		return this->time_passed_ >= target_time_;
	}

	void Timer::reset_to_surplus()
	{
		if(time_passed_ >= target_time_)
		{
			time_passed_ -= target_time_;
		}
	}

	void Timer::reset()
	{
		time_passed_ = Seconds{0.0};
	}
}

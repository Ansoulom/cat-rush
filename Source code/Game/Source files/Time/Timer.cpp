#include "Timer.h"
#include <chrono>

namespace Game
{
	Timer::Timer(Seconds max_time) : time_passed{0.0}, target_time{max_time}
	{
	}

	bool Timer::update(Seconds time_passed)
	{
		this->time_passed += time_passed;

		return this->time_passed >= target_time;
	}

	void Timer::reset_to_surplus()
	{
		if(time_passed >= target_time)
		{
			time_passed -= target_time;
		}
	}

	void Timer::reset()
	{
		time_passed = Seconds{0.0};
	}
}

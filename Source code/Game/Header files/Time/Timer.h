#pragma once

#include <chrono>

namespace Game
{
	class Timer
	{
	public:
		using Seconds = std::chrono::duration<double, std::ratio<1>>;

		Timer(Seconds max_time); // max_time is how long time before the timer is done.
		
		bool update(Seconds time_passed); // Updates the timer and returns if it is done.
		void reset_to_surplus(); // Resets the time, but keeps the surplus time over max time.
		void reset(); // Completely resets the timer to zero.

	private:
		Seconds time_passed;
		const Seconds target_time;
	};
}

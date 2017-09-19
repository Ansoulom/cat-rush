#pragma once
#include "Timer.h"

namespace Game
{
	class Frame_timer
	{
	public:
		Frame_timer();

		std::chrono::nanoseconds update(); // Should be run every frame. Returns delta time after the update.
		std::chrono::nanoseconds get_delta_time() const;
		int get_framerate() const; // Returns a new value once every second.

	private:
		int framerate, frames;
		std::chrono::nanoseconds time_passed, delta_time;
		std::chrono::time_point<std::chrono::high_resolution_clock> previous_time;
	};
}

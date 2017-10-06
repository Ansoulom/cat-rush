#pragma once

#include "Timer.h"
#include <Component.h>


namespace Game
{
	namespace Input
	{
		class Input_handler;
	}


	namespace Objects
	{
		class Game_object;


		class Input_component : public Component
		{
		public:
			explicit Input_component(Game_object* container);

			virtual void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) = 0;
		};


		enum class Direction
		{
			left = -1, right = 1
		};
	}
}

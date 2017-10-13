#pragma once

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

		// TODO: Just remove this class?
		class Input_component : public Component
		{
		public:
			explicit Input_component(Game_object& game_object);
		};
	}
}

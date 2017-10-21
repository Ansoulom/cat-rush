#pragma once


#include "Component.h"


namespace Game
{
	namespace Objects
	{
		class Noodles_ai_component : public Component
		{
		public:
			explicit Noodles_ai_component(Game_object& game_object);
		};
	}
}

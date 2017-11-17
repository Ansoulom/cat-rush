#include "Randomness.h"


namespace Game
{
	namespace Random
	{
		std::default_random_engine& default_engine()
		{
			static auto engine = std::default_random_engine{};
			return engine;
		}
	}
}

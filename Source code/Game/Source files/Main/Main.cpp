#include "Game_core.h"


int main(int argc, char* args[])
{
	/*try
	{
		//auto game = game::Game_core{ "Tidshax" }; // Guaranteed copy ellision allows for this to work in C++17
		Game::Game_core game{"Chronos?"};
		game.run();
	}
	catch(std::exception& e)
	{
		Game::Logger::log(e.what());
		throw;
	}
	catch(...)
	{
		Game::Logger::log("Undefined exception caught in main.");
		throw;
	}/**/

	Game::Game_core game{"Chronos?"};
	game.run();

	return 0;
}

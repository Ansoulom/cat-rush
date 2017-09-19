#pragma once

#include <iostream>


namespace Game
{
	// TODO: Maybe redo to use global functions instead or at least prevent instances. Or make it a singleton.
	class Logger
	{
	public:
		Logger();
		~Logger();
		static void log(std::string message);
	};
}

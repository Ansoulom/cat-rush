#include <string>
#include "Logger.h"

namespace Game
{
	Logger::Logger()
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::log(std::string message)
	{
		std::cout << message << std::endl;
	}
}
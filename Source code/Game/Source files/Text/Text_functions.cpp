#include "Text_functions.h"
#include <algorithm>
#include <cctype>


namespace Game
{
	namespace Text
	{
		void trim(std::string& text)
		{
			text.erase(remove_if(text.begin(), text.end(), [](unsigned char c) { return isspace(c); }), text.end());
		}


		void trim(std::string& text, char character)
		{
			text.erase(remove(text.begin(), text.end(), character), text.end());
		}
	}
}

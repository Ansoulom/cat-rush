#include <regex>
#include <sstream>
#include <cctype>
#include "IO/INI.h"


namespace Game
{
	namespace Text
	{
		void trim(std::string& text)
		{
			text.erase(remove_if(text.begin(), text.end(), [](unsigned char c) { return std::isspace(c); }), text.end());
		}
	}


	namespace Io
	{
		Ini::Ini(const std::string& document)
		{
			// regular expressions to process ini files
			const auto section_test = std::regex{"\\[(.*?)\\]"};
			const auto value_test = std::regex{"(\\w+)=([^\\+]+(?!\\+{3}))"};

			auto stream = std::istringstream{document};
			std::string line, current;
			while(getline(stream, line))
			{
				Text::trim(line);

				if(line.length() > 0)
				{
					if(line[0] == ';') continue;

					std::smatch match;
					if(regex_search(line, match, section_test))
					{
						current = match[1];
					}
					else if(regex_search(line, match, value_test))
					{
						content_[current][match[1]] = match[2];
					}
					else
					{
						throw std::runtime_error{std::string{"Incorrect line in INI file: "} + line};
					}
				}
			}
		}


		std::string_view Ini::value(const std::string& section, const std::string& key) const
		{
			return content_.at(section).at(key);
		}
	}
}

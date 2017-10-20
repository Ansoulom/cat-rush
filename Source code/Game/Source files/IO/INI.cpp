#include <regex>
#include <sstream>
#include "IO/INI.h"
#include "Text_functions.h"


namespace Game
{
	namespace Io
	{
		Ini::Ini(const std::string& document)
		{
			// regular expressions to process ini files
			const auto section_test = std::regex{ R"(\[(.*?)\])" };
			const auto value_test = std::regex{ R"((\w+)=(.+))" };

			auto stream = std::istringstream{document};
			std::string line, current;
			while(getline(stream, line))
			{
				Text::trim(line, '\0');
				if(line.empty() || line[0] == ';') continue;

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


		const std::string& Ini::value(const std::string& section, const std::string& key) const
		{
			return content_.at(section).at(key);
		}


		Ini load_ini_from_file(const boost::filesystem::path& file)
		{
			std::ifstream in{file.string()};
			if(!in.is_open())
			{
				throw std::runtime_error{std::string{"Could not open settings file"} + file.string()};
			}
			auto document = std::string{};
			in.seekg(0, std::ios::end);
			document.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&document[0], document.size());
			in.close();
			return Ini{document};
		}
	}
}

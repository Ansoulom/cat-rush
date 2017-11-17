#include "JSON_extensions.h"
#include <fstream>


namespace Game
{
	namespace Io
	{
		json load_json_from_file(const boost::filesystem::path file)
		{
			std::ifstream in{file.string()};
			if(!in.is_open())
			{
				throw std::runtime_error{std::string{"Could not open json file"} + file.string()};
			}

			auto json_object = json{};
			in >> json_object;

			return json_object;
		}
	}
}

#pragma once

#include <boost/filesystem/path.hpp>
#include <json.hpp>


namespace Game
{
	namespace Io
	{
		using namespace nlohmann;

		json load_json_from_file(const boost::filesystem::path file);
	}
}

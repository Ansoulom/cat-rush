#pragma once

#include <filesystem>
#include <json.hpp>


namespace Game
{
	namespace Io
	{
		using namespace nlohmann;

		json load_json_from_file(const std::filesystem::path file);
	}
}

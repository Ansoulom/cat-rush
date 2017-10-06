#include "Settings.h"
#include "json.hpp"
#include "IO/INI.h"


namespace Game
{
	Settings::Settings(int resolution_width, int resolution_height, int source_width, int source_height) :
		resolution_width_{resolution_width}, resolution_height_{resolution_height}, source_width_{source_width},
		source_height_{source_height} { }


	int Settings::resolution_width() const
	{
		return resolution_width_;
	}


	int Settings::resolution_height() const
	{
		return resolution_height_;
	}


	int Settings::source_width() const
	{
		return source_width_;
	}


	int Settings::source_height() const
	{
		return source_height_;
	}


	Settings load_settings(const boost::filesystem::path& settings_file)
	{
		std::ifstream in{settings_file.string()};
		if(!in.is_open())
		{
			throw std::runtime_error{std::string{"Could not open level file"} + settings_file.string()};
		}
		auto document = std::string{};
		in >> document;
		auto ini = Io::Ini{document};

		const auto resolution_width = ini.value<int>("graphics", "resolution_width");
		const auto resolution_height = ini.value<int>("graphics", "resolution_height");
		const auto source_width = ini.value<int>("graphics", "source_width");
		const auto source_height = ini.value<int>("graphics", "source_width");
		return {resolution_width, resolution_height, source_width, source_height};
	}
}

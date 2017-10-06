#pragma once
#include <boost/filesystem.hpp>


namespace Game
{
	class Settings
	{
	public:
		Settings(int resolution_width, int resolution_height, int source_width, int source_height);
		int resolution_width() const;
		int resolution_height() const;
		int source_width() const;
		int source_height() const;
		
	private:
		int resolution_width_, resolution_height_, source_width_, source_height_;
	};

	
	Settings load_settings(const boost::filesystem::path& settings_file);
}

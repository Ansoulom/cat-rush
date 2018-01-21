#include "Settings.h"
#include "JSON_extensions.h"
#include "IO/INI.h"


namespace Game
{
	namespace Core
	{
		User_settings::User_settings(const boost::filesystem::path& file)
		{
			auto ini = Io::load_ini_from_file(file);
			resolution_width_ = ini.value<int>("graphics", "resolution_width");
			resolution_height_ = ini.value<int>("graphics", "resolution_height");
			vsync_ = ini.value<bool>("graphics", "vsync");

			auto fullscreen_mode = ini.value<int>("graphics", "fullscreen_mode");
			if(fullscreen_mode < 0 || fullscreen_mode > 2) throw std::runtime_error{"Incorrect value in configuration file!"};
			fullscreen_mode_ = static_cast<Window_fullscreen_mode>(fullscreen_mode);
		}


		int User_settings::resolution_width() const
		{
			return resolution_width_;
		}


		int User_settings::resolution_height() const
		{
			return resolution_height_;
		}


		Window_fullscreen_mode User_settings::fullscreen_mode() const
		{
			return fullscreen_mode_;
		} 


		Game_constants::Game_constants(const boost::filesystem::path& file)
		{
			auto ini = Io::load_ini_from_file(file);
			name_ = ini.value<std::string>("window", "name");
			source_width_ = ini.value<int>("graphics", "source_resolution_width");
			source_height_ = ini.value<int>("graphics", "source_resolution_height");
		}


		std::string Game_constants::name() const
		{
			return name_;
		}


		int Game_constants::source_width() const
		{
			return source_width_;
		}


		int Game_constants::source_height() const
		{
			return source_height_;
		}


		Settings::Settings(
			const boost::filesystem::path& game_constants_file,
			const boost::filesystem::path& user_settings_file)
			: constants_{game_constants_file}, settings_{user_settings_file} { }


		Game_constants& Settings::constants()
		{
			return constants_;
		}


		const Game_constants& Settings::constants() const
		{
			return constants_;
		}


		User_settings& Settings::user_settings()
		{
			return settings_;
		}


		const User_settings& Settings::user_settings() const
		{
			return settings_;
		}


		bool User_settings::vsync() const
		{
			return vsync_;
		}
	}
}

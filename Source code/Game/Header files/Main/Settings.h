#pragma once

#include <boost/filesystem.hpp>
#include "Window.h"


namespace Game
{
	namespace Core
	{
		class User_settings
		{
		public:
			explicit User_settings(const boost::filesystem::path& file);
			int resolution_width() const;
			int resolution_height() const;
			Window_fullscreen_mode fullscreen_mode() const;
			bool vsync() const;

		private:
			int resolution_width_{}, resolution_height_{};
			Window_fullscreen_mode fullscreen_mode_{};
			bool vsync_{};
		};


		class Game_constants
		{
		public:
			explicit Game_constants(const boost::filesystem::path& file);

			std::string name() const;
			int source_width() const;
			int source_height() const;
			

		private:
			std::string name_{};
			int source_width_{}, source_height_{};
		};


		class Settings
		{
		public:
			Settings(const boost::filesystem::path& game_constants_file, const boost::filesystem::path& user_settings_file);

			Game_constants& constants();
			const Game_constants& constants() const;
			User_settings& user_settings();
			const User_settings& user_settings() const;

		private:
			Game_constants constants_;
			User_settings settings_;
		};
	}
}

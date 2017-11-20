#pragma once

#include "Texture.h"
#include <unordered_map>
#include "Fonts.h"


namespace boost
{
	namespace filesystem
	{
		class path;
	}
}


namespace Game
{
	namespace Graphics
	{
		class Renderer;
	}


	namespace Resources
	{
		class Texture_manager
		{
		public:
			Texture_manager(Graphics::Renderer& renderer);

			Texture_manager(const Texture_manager& other) = delete;
			Texture_manager(Texture_manager&& other) = default;

			Texture_manager& operator=(const Texture_manager& other) = delete;
			Texture_manager& operator=(Texture_manager&& other) = default;

			const Graphics::Texture& get_texture(std::string texture_name) const;

		private:
			void load_all_textures(const boost::filesystem::path& texture_path, Graphics::Renderer& renderer);

			std::unordered_map<std::string, Graphics::Texture> textures_;
		};


		class Font_manager
		{
		public:
			Font_manager();

			const Text::Font& get_font(const std::string& font_name) const;

		private:
			void load_all_fonts(const boost::filesystem::path& font_path);

			std::unordered_map<std::string, Text::Font> fonts_;
		};


		class Resource_manager
		{
		public:
			explicit Resource_manager(Graphics::Renderer& renderer);
			// TODO: Fix constructor somehow
			Texture_manager& textures();
			const Texture_manager& textures() const;
			Font_manager& fonts();
			const Font_manager& fonts() const;

		private:
			Texture_manager textures_;
			Font_manager fonts_{};
		};
	}
}

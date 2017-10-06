#pragma once
#include "Texture.h"
#include <unordered_map>


namespace Game
{
	namespace Graphics
	{
		class Renderer;
	}
}


namespace boost
{
	namespace filesystem
	{
		class path;
	}
}


namespace Game
{
	namespace Resources
	{
		class Texture_manager
		{
		public:
			Texture_manager();

			void load_all_textures(const boost::filesystem::path& texture_path, Graphics::Renderer& renderer);
			std::shared_ptr<Graphics::Texture> get_texture(std::string texture_name) const;

		private:
			std::unordered_map<std::string, std::shared_ptr<Graphics::Texture>> textures_;
		};


		class Font_manager { };


		class Resource_manager
		{
		public:
			// TODO: Fix constructor somehow
			Texture_manager& textures();
			const Texture_manager& textures() const;
			Font_manager& fonts();
			const Font_manager& fonts() const;

		private:
			Texture_manager textures_;
			Font_manager fonts_;
		};
	}
}

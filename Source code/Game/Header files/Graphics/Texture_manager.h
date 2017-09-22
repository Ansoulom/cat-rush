#pragma once

#include <boost/filesystem.hpp>
#include <unordered_map>
#include <memory>
#include "Texture.h"


namespace Game
{
	namespace Graphics
	{
		class Texture_manager
		{
		public:
			Texture_manager();
			~Texture_manager();

			bool load_all_textures(const boost::filesystem::path& texture_path, SDL_Renderer& renderer);
			std::shared_ptr<Texture> get_texture(std::string texture_name) const;
		private:
			std::unordered_map<std::string, std::shared_ptr<Texture>> textures_;
		};
	}
}

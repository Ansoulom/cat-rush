#include "Texture_manager.h"
#include <SDL.h>


namespace Game
{
	namespace Graphics
	{
		Texture_manager::Texture_manager() : textures_{}
		{
		}


		Texture_manager::~Texture_manager()
		{
		}


		bool Texture_manager::load_all_textures(const boost::filesystem::path& texture_path, SDL_Renderer& renderer)
		{
			// TODO: Be careful about error handling
			auto success = true;
			if(exists(texture_path) && is_directory(texture_path))
			{
				for(auto&& x : boost::filesystem::recursive_directory_iterator{texture_path})
				{
					if(x.path().extension().string() == ".png")
					{
						auto texture = std::make_shared<Texture>(renderer, x.path().string());
						auto filename = x.path().stem().string();
						textures_.emplace(filename, texture);
					}
				}
			}
			return success;
		}


		std::shared_ptr<Texture> Texture_manager::get_texture(std::string texture_name) const
		{
			return textures_.at(texture_name); // TODO: Decide what to do if texture does not exist
		}
	}
}

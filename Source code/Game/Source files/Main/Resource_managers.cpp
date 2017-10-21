#include "Resource_managers.h"
#include <boost/filesystem/operations.hpp>


namespace Game
{
	namespace Resources
	{
		Texture_manager::Texture_manager() : textures_{} { }


		void Texture_manager::load_all_textures(const boost::filesystem::path& texture_path, Graphics::Renderer& renderer)
		{
			// TODO: Be careful about error handling
			if(exists(texture_path) && is_directory(texture_path))
			{
				for(auto&& x : boost::filesystem::recursive_directory_iterator{texture_path})
				{
					if(x.path().extension().string() == ".png")
					{
						auto texture = std::make_shared<Graphics::Texture>(renderer, x.path().string());
						auto filename = x.path().stem().string();
						textures_.emplace(filename, texture);
					}
				}
			}
		} 


		std::shared_ptr<Graphics::Texture> Texture_manager::get_texture(std::string texture_name) const
		{
			return textures_.at(texture_name); // TODO: Decide what to do if texture does not exist
		} 


		Texture_manager& Resource_manager::textures()
		{
			return textures_;
		}


		const Texture_manager& Resource_manager::textures() const
		{
			return textures_;
		}


		Font_manager& Resource_manager::fonts()
		{
			return fonts_;
		}


		const Font_manager& Resource_manager::fonts() const
		{
			return fonts_;
		}
	}
}

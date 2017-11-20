#include "Resource_managers.h"
#include <boost/filesystem/operations.hpp>
#include "Renderer.h"
#include "File_paths.h"


namespace Game
{
	namespace Resources
	{
		Texture_manager::Texture_manager(Graphics::Renderer& renderer)
			: textures_{}
		{
			load_all_textures(boost::filesystem::path{Io::Paths::textures}, renderer);
		}


		void Texture_manager::load_all_textures(const boost::filesystem::path& texture_path, Graphics::Renderer& renderer)
		{
			// TODO: Be careful about error handling
			if(exists(texture_path) && is_directory(texture_path))
			{
				for(auto&& x : boost::filesystem::recursive_directory_iterator{texture_path})
				{
					if(x.path().extension().string() == ".png")
					{
						textures_.emplace(x.path().stem().string(), Graphics::Texture{renderer, x.path().string()});
					}
				}
			}
		}


		const Graphics::Texture& Texture_manager::get_texture(std::string texture_name) const
		{
			return textures_.at(texture_name); // TODO: Decide what to do if texture does not exist
		}


		Font_manager::Font_manager()
		{
			load_all_fonts(boost::filesystem::path{Io::Paths::fonts});
		}


		void Font_manager::load_all_fonts(const boost::filesystem::path& font_path)
		{
			if(exists(font_path) && is_directory(font_path))
			{
				for(auto&& x : boost::filesystem::recursive_directory_iterator{font_path})
				{
					if(x.path().extension().string() == ".ttf")
					{
						fonts_.emplace(x.path().stem().string(), Text::Font{x.path().string(), 12}); // TODO: Get rid of magic number
					}
				}
			}
		}


		const Text::Font& Font_manager::get_font(const std::string& font_name) const
		{
			return fonts_.at(font_name);
		}


		Resource_manager::Resource_manager(Graphics::Renderer& renderer)
			: textures_{renderer} {}


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

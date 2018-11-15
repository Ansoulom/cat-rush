#include "Texture.h"
#include "Game_core.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Colors.h"


namespace Game
{
	namespace Graphics
	{
		Texture::Texture(Renderer& renderer, const std::filesystem::path& file_path)
			: Texture{
				std::unique_ptr<SDL_Surface, Sdl_deleter>{IMG_Load(file_path.string().c_str()), Sdl_deleter{}},
				renderer
			} { }


		Texture::Texture(Renderer& renderer, const std::string& text, Color text_color, const Text::Font& font)
			: Texture{
				std::unique_ptr<SDL_Surface, Sdl_deleter>{
					TTF_RenderText_Blended(
						&Wrappers::Sdl_wrapper::get_font(font),
						text.c_str(),
						Wrappers::Sdl_wrapper::get_color(text_color)),
					Sdl_deleter{}
				},
				renderer
			} { }


		Texture::Texture(Texture&& other) noexcept
			: texture_{move(other.texture_)}, width_{other.width_}, height_{other.height_} { }


		Texture& Texture::operator=(Texture&& other) noexcept
		{
			texture_ = move(other.texture_);
			width_ = other.width_;
			height_ = other.height_;

			return *this;
		}


		Texture::Texture(std::unique_ptr<SDL_Surface, Sdl_deleter> surface, Renderer& renderer)
			: texture_{}, width_{0}, height_{0}
		{
			if(!surface)
			{
				throw std::runtime_error{std::string{"Could not create surface: "} + SDL_GetError()};
			}
			texture_ = std::unique_ptr<SDL_Texture, Sdl_deleter>{
				SDL_CreateTextureFromSurface(renderer.sdl_renderer_.get(), surface.get()),
				Sdl_deleter{}
			};
			if(!texture_)
			{
				throw std::runtime_error{std::string{"Could not create texture from surface: "} + SDL_GetError()};
			}
			SDL_SetTextureBlendMode(texture_.get(), SDL_BLENDMODE_BLEND);
			width_ = surface->w;
			height_ = surface->h;
		}


		int Texture::width() const
		{
			return width_;
		}


		int Texture::height() const
		{
			return height_;
		}
	}
}

#include "Texture.h"
#include "Game_core.h"
#include <SDL_image.h>
#include <SDL_ttf.h>


namespace Game
{
	namespace Graphics
	{
		Texture::Texture(SDL_Renderer& renderer, std::string file_path) : Texture{std::unique_ptr<SDL_Surface, SDL_deleter>{IMG_Load(file_path.c_str()), SDL_deleter{}}, renderer}
		{
		}


		Texture::Texture(SDL_Renderer& renderer, std::string text, SDL_Color text_color, TTF_Font& font) : Texture{std::unique_ptr<SDL_Surface, SDL_deleter>{TTF_RenderText_Blended(&font, text.c_str(), text_color), SDL_deleter{}}, renderer}
		{
		}


		Texture::Texture(std::unique_ptr<SDL_Surface, SDL_deleter> surface, SDL_Renderer& renderer) : renderer{&renderer}, texture{}, width{0}, height{0}
		{
			if(!surface)
			{
				throw std::runtime_error{std::string{"Could not create surface: "} + SDL_GetError()};
			}
			texture = std::unique_ptr<SDL_Texture, SDL_deleter>{SDL_CreateTextureFromSurface(&renderer, surface.get()), SDL_deleter{}};
			if(!texture)
			{
				throw std::runtime_error{std::string{"Could not create texture from surface: "} + SDL_GetError()};
			}
			SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
			width = surface->w;
			height = surface->h;
		}


		Texture::~Texture()
		{
		}


		int Texture::get_width() const
		{
			return width;
		}


		int Texture::get_height() const
		{
			return height;
		}


		void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const
		{
			auto render_quad = SDL_Rect{x, y, width, height};
			if(clip)
			{
				render_quad.w = clip->w;
				render_quad.h = clip->h;
			}
			if(SDL_RenderCopyEx(renderer, texture.get(), clip, &render_quad, angle, center, flip))
			{
				throw std::runtime_error{std::string{"Could not render texture: "} + SDL_GetError()};
			}
		}


		void Texture::set_color(Uint8 red, Uint8 green, Uint8 blue)
		{
			SDL_SetTextureColorMod(texture.get(), red, green, blue);
		}


		void Texture::set_alpha(Uint8 alpha)
		{
			SDL_SetTextureAlphaMod(texture.get(), alpha);
		}
	}
}

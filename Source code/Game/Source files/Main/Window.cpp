#include "Window.h"


namespace Game
{
	namespace Core
	{
		Window::Window(
			std::string name,
			int x,
			int y,
			int width,
			int height,
			Window_fullscreen_mode fullscreen_mode,
			bool visible)
			: sdl_window_{
				SDL_CreateWindow(
					name.c_str(),
					x,
					y,
					width,
					height,
					(visible ? 0 : SDL_WINDOW_HIDDEN) | (fullscreen_mode == Window_fullscreen_mode::fullscreen ?
															 SDL_WINDOW_FULLSCREEN :
															 fullscreen_mode == Window_fullscreen_mode::borderless ?
															 SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED :
															 0)),
				Sdl_deleter{}
			}
		{
			if(!sdl_window_)
			{
				throw std::runtime_error{std::string{"Window could not be created: "} + SDL_GetError()};
			}
		}


		void Window::set_title(const std::string& title)
		{
			SDL_SetWindowTitle(sdl_window_.get(), title.c_str());
		}


		void Window::set_visible(bool visible)
		{
			if(visible) SDL_ShowWindow(sdl_window_.get());
			else SDL_HideWindow(sdl_window_.get());
		}
	}
}

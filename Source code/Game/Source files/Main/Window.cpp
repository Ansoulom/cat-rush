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


		void Window::set_position(Geometry::Vector<int> position)
		{
			SDL_SetWindowPosition(sdl_window_.get(), position.get_x(), position.get_y());
		}


		Geometry::Vector<int> Window::get_position() const
		{
			auto pos = Geometry::Vector<int>{};
			SDL_GetWindowPosition(sdl_window_.get(), &pos.x(), &pos.y());
			return pos;
		}


		void Window::set_size(Geometry::Vector<int> size)
		{
			SDL_SetWindowSize(sdl_window_.get(), size.get_x(), size.get_y());
		}


		Geometry::Vector<int> Window::get_size() const
		{
			auto size = Geometry::Vector<int>{};
			SDL_GetWindowSize(sdl_window_.get(), &size.x(), &size.y());
			return size;
		}
	}
}

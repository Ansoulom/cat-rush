#include "Renderer.h"
#include "Camera.h"
#include "Render_instance.h"
#include "Game_core.h"
#include "Graphics_component.h"
#include "Window.h"


namespace Game
{
	namespace Graphics
	{
		Renderer::Renderer(const Core::Settings& settings, Core::Window& window)
			: settings_{settings},
			  sdl_renderer_{
				  SDL_CreateRenderer(
					  window.sdl_window_.get(),
					  -1,
					  SDL_RENDERER_ACCELERATED | (settings.user_settings().vsync() ? SDL_RENDERER_PRESENTVSYNC : 0)),
				  Sdl_deleter{}
			  }
		{
			if(!sdl_renderer_)
			{
				throw std::runtime_error{std::string{"Renderer could not be created! SDL Error: "} + SDL_GetError()};
			}
		}


		void Renderer::render(const Resources::Texture_manager& tm, const Camera& camera) const
		// Don't really like having SDL_Renderer as a parameter. TODO: Wrap SDL_Renderer in Renderer
		{
			auto render_queue = std::vector<Render_instance>{};
			auto camera_box = camera.bounding_box();
			const auto start_x = static_cast<int>(camera_box.left() / grid_cell_size_) - 1;
			const auto end_x = static_cast<int>(camera_box.right() / grid_cell_size_) + 1;
			const auto start_y = static_cast<int>(camera_box.top() / grid_cell_size_) - 1;
			const auto end_y = static_cast<int>(camera_box.bottom() / grid_cell_size_) + 1;

			for(auto x = start_x; x <= end_x; ++x)
			{
				for(auto y = start_y; y <= end_y; ++y)
				{
					const auto cell_pos = Geometry::Vector<int>{x, y};
					auto iter = grid_.find(cell_pos);
					if(iter != grid_.end())
					{
						for(auto component : iter->second)
						{
							render_queue.push_back(component->get_render_instance(tm, camera));
						}
					}
				}
			}

			sort(
				render_queue.begin(),
				render_queue.end(),
				[this](const Render_instance& a, const Render_instance& b)
			{
				return a.destination_.get_y() + a.source_.height() / 2 / settings_.constants().source_height() < b.destination_.
																												   get_y() + b.source_.height() / 2;
			}); // TODO: Make this work with different layers or such, for example object on ground

			for(const auto& instance : render_queue)
			{
				render(instance);
			}
		}


		void Renderer::register_component(Objects::Graphics_component& gc)
		{
			auto cell_x = static_cast<int>(gc.game_object().position().get_x() / grid_cell_size_);
			auto cell_y = static_cast<int>(gc.game_object().position().get_y() / grid_cell_size_);
			auto cell_pos = Geometry::Vector<int>{cell_x, cell_y};

			grid_[cell_pos].push_back(&gc);

			gc.add_destroy_listener(bind(&Renderer::remove_component, this, std::placeholders::_1));
		}


		void Renderer::remove_component(Objects::Graphics_component& gc)
		{
			auto cell_x = static_cast<int>(gc.game_object().position().get_x() / grid_cell_size_);
			auto cell_y = static_cast<int>(gc.game_object().position().get_y() / grid_cell_size_);
			auto cell_pos = Geometry::Vector<int>{cell_x, cell_y};
			auto iter = grid_.find(cell_pos);
			if(iter != grid_.end())
			{
				iter->second.erase(remove(iter->second.begin(), iter->second.end(), &gc));
			}
		}


		Aspect_ratio::Aspect_ratio(int x, int y) : x_{x}, y_{y} { }


		int Aspect_ratio::x() const
		{
			return x_;
		}


		int Aspect_ratio::y() const
		{
			return y_;
		}


		double Aspect_ratio::x_multiplier() const
		{
			return static_cast<double>(x_) / static_cast<double>(y_);
		}


		double Aspect_ratio::y_multiplier() const
		{
			return 1.0;
		}


		void Renderer::render(const Render_instance& instance) const
		{
			const auto screen_dest = Geometry::Rectangle<int>{
				{
					static_cast<int>(round(instance.destination_.get_x() * settings_.constants().source_height())),
					static_cast<int>(round(instance.destination_.get_y() * settings_.constants().source_height()))
				},
				static_cast<int>(round(instance.scaling_.get_x() * instance.source_.width())),
				static_cast<int>(round(instance.scaling_.get_y() * instance.source_.height()))
			};
			const auto screen_pivot = Geometry::Vector<int>{
				static_cast<int>(round(instance.pivot_.get_x() * screen_dest.width())),
				static_cast<int>(round(instance.pivot_.get_y() * screen_dest.height()))
			};

			auto dst = Wrappers::Sdl_wrapper::get_rect(screen_dest);
			auto src = Wrappers::Sdl_wrapper::get_rect(instance.source_);
			auto center = Wrappers::Sdl_wrapper::get_point(screen_pivot);
			const auto flip_x = instance.flipped_x_ ? SDL_FLIP_HORIZONTAL : 0;
			const auto flip_y = instance.flipped_y_ ? SDL_FLIP_VERTICAL : 0;

			if(SDL_RenderCopyEx(
				sdl_renderer_.get(),
				instance.texture_->texture_.get(),
				&src,
				&dst,
				instance.angle_,
				&center,
				static_cast<SDL_RendererFlip>(flip_x | flip_y)))
			{
				throw std::runtime_error{std::string{"Could not render texture: "} + SDL_GetError()};
			}
		}


		void Renderer::clear(Color clear_color)
		{
			SDL_SetRenderDrawColor(
				sdl_renderer_.get(),
				clear_color.red(),
				clear_color.green(),
				clear_color.blue(),
				clear_color.alpha());
			SDL_RenderClear(sdl_renderer_.get());
		}


		void Renderer::show()
		{
			SDL_RenderPresent(sdl_renderer_.get());
		}


		void Renderer::set_render_scale(double x, double y)
		{
			SDL_RenderSetScale(sdl_renderer_.get(), static_cast<float>(x), static_cast<float>(y));
		}
	}
}

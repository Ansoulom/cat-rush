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
		Geometry::Vector<int> Render_settings::normalized_to_pixels(Geometry::Vector<double> normalized) const
		{
			return {
				normalized_to_pixels(normalized.get_x()),
				normalized_to_pixels(normalized.get_y())
			};
		}


		int Render_settings::normalized_to_pixels(double normalized) const
		{
			return static_cast<int>(round(normalized * render_size.get_y()));
		}


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


		Render_settings Renderer::render_settings() const
		{
			return {{settings_.constants().source_width(), settings_.constants().source_height()}};
		}


		Aspect_ratio::Aspect_ratio(int x, int y)
			: x_{x}, y_{y} { }


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


		Render_grid::Render_grid()
			: component_destroyed_receiver_{bind(&Render_grid::remove_component, this, std::placeholders::_1)},
			  component_moved_receiver_{
				  bind(&Render_grid::update_grid_position, this, std::placeholders::_1, std::placeholders::_2)
			  } { }


		std::vector<Render_instance> Render_grid::get_render_instances(
			const Resources::Texture_manager& tm,
			const Render_settings& render_settings,
			const Camera& camera) const
		{
			auto render_queue = std::vector<Render_instance>{};
			auto camera_box = camera.bounding_box();
			const auto start_x = static_cast<int>(camera_box.left() / grid_cell_size) - 1;
			const auto end_x = static_cast<int>(camera_box.right() / grid_cell_size) + 1;
			const auto start_y = static_cast<int>(camera_box.top() / grid_cell_size) - 1;
			const auto end_y = static_cast<int>(camera_box.bottom() / grid_cell_size) + 1;

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
							render_queue.push_back(component->get_render_instance(tm, camera, render_settings));
						}
					}
				}
			}

			sort(
				render_queue.begin(),
				render_queue.end(),
				[this](const Render_instance& a, const Render_instance& b)
				{
					return a.layer_ != b.layer_ ?
							   a.layer_ < b.layer_ :
							   a.destination_.bottom() < b.destination_.bottom();
				});

			return render_queue;
		}


		void Render_grid::register_component(Objects::Graphics_component& gc)
		{
			put_in_grid(gc, get_grid_cell_position(gc.game_object().position()));
			gc.add_destroy_receiver(component_destroyed_receiver_);
		}


		void Render_grid::remove_component(Objects::Graphics_component& gc)
		{
			remove_from_grid(gc, get_grid_cell_position(gc.game_object().position()));
		}


		void Render_grid::update_grid_position(
			Objects::Graphics_component& component,
			Geometry::Vector<double> old_position)
		{
			const auto old_cell_pos = get_grid_cell_position(old_position);
			const auto cell_pos = get_grid_cell_position(component.game_object().position());

			if(old_cell_pos != cell_pos)
			{
				remove_from_grid(component, old_cell_pos);
				put_in_grid(component, cell_pos);
			}
		}


		void Render_grid::put_in_grid(Objects::Graphics_component& comp, Geometry::Vector<int> grid_cell_position)
		{
			grid_[grid_cell_position].push_back(&comp);
		}


		void Render_grid::remove_from_grid(Objects::Graphics_component& comp, Geometry::Vector<int> grid_cell_pos)
		{
			auto iter = grid_.find(grid_cell_pos);
			assert(iter != grid_.end());
			iter->second.erase(remove(iter->second.begin(), iter->second.end(), &comp), iter->second.end());
		}


		Geometry::Vector<int> Render_grid::get_grid_cell_position(Geometry::Vector<double> object_position)
		{
			const auto cell_x = static_cast<int>(object_position.get_x() / grid_cell_size);
			const auto cell_y = static_cast<int>(object_position.get_y() / grid_cell_size);
			return {cell_x, cell_y};
		}


		const double Render_grid::grid_cell_size{10.0}; // VERY TEMPORARY VALUE


		std::vector<Render_instance> merge_render_lists(std::initializer_list<std::vector<Render_instance>> lists)
		{
			auto instances = std::vector<Render_instance>{};
			for(auto list : lists)
			{
				instances.insert(instances.end(), list.begin(), list.end());
			}

			return instances;
		}


		void Renderer::render(const Render_instance& instance) const
		{
			const auto screen_dest = instance.destination_;
			const auto screen_pivot = instance.pivot_;

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

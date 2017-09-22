#include "Renderer.h"
#include "Camera.h"
#include "Render_instance.h"
#include "Game_core.h"
#include "Graphics_component.h"


namespace Game
{
	namespace Graphics
	{
		Renderer::Renderer()
		{
		}


		void Renderer::render(const Texture_manager& tm, const Camera& camera, SDL_Renderer& sdl_renderer) const // Don't really like having SDL_Renderer as a parameter. TODO: Wrap SDL_Renderer in Renderer
		{
			auto render_queue = std::vector<Render_instance>{};
			auto camera_box = camera.bounding_box();
			auto start_x = static_cast<int>(camera_box.left() / grid_cell_size_) - 1;
			auto end_x = static_cast<int>(camera_box.right() / grid_cell_size_) + 1;
			auto start_y = static_cast<int>(camera_box.top() / grid_cell_size_) - 1;
			auto end_y = static_cast<int>(camera_box.bottom() / grid_cell_size_) + 1;

			for(auto x = start_x; x <= end_x; ++x)
			{
				for(auto y = start_y; y <= end_y; ++y)
				{
					auto cell_pos = Geometry::Vector<int>{ x, y };
					auto iter = grid_.find(cell_pos);
					if(iter != grid_.end())
					{
						for(auto component : iter->second)
						{
							auto render_instance = component->get_render_instance(tm, camera);
							if(intersects(Geometry::Rectangle<int>{{0, 0}, Game_core::source_width, Game_core::source_height, Geometry::Pivot_point::top_left}, render_instance.destination_))
							{
								render_queue.push_back(render_instance);
							}
						}
					}
				}
			}

			std::sort(render_queue.begin(), render_queue.end(), [](const Render_instance& a, const Render_instance& b) { return a.destination_.bottom() < b.destination_.bottom(); }); // TODO: Make this work with different layers or such, for example object on ground

			for(const auto& instance : render_queue)
			{
				render(instance, sdl_renderer);
			}
		}


		void Renderer::register_component(Objects::Graphics_component& gc)
		{
			auto cell_x = static_cast<int>(gc.get_position().get_x() / grid_cell_size_);
			auto cell_y = static_cast<int>(gc.get_position().get_y() / grid_cell_size_);
			auto cell_pos = Geometry::Vector<int>{ cell_x, cell_y };

			grid_[cell_pos].push_back(&gc);

			gc.add_destroy_listener(bind(&Renderer::remove_component, this, std::placeholders::_1));
		}


		void Renderer::remove_component(Objects::Graphics_component& gc)
		{
			auto cell_x = static_cast<int>(gc.get_position().get_x() / grid_cell_size_);
			auto cell_y = static_cast<int>(gc.get_position().get_y() / grid_cell_size_);
			auto cell_pos = Geometry::Vector<int>{ cell_x, cell_y };
			auto iter = grid_.find(cell_pos);
			if(iter != grid_.end())
			{
				iter->second.erase(remove(iter->second.begin(), iter->second.end(), &gc));
			}
		}


		void Renderer::render(const Render_instance& instance, SDL_Renderer& sdl_renderer) const
		{
			auto dst = Sdl_wrapper::get_rect(instance.destination_);
			auto src = Sdl_wrapper::get_rect(instance.source_);
			auto center = Sdl_wrapper::get_point(instance.pivot_);
			auto flip_x = instance.flipped_x_ ? SDL_FLIP_HORIZONTAL : 0;
			auto flip_y = instance.flipped_y_ ? SDL_FLIP_VERTICAL : 0;

			if(SDL_RenderCopyEx(&sdl_renderer, instance.texture_->texture_.get(), &src, &dst, instance.angle_, &center, static_cast<SDL_RendererFlip>(flip_x | flip_y)))
			{
				throw std::runtime_error{std::string{"Could not render texture: "} + SDL_GetError()};
			}
		}
	}
}

#pragma once

#include <list>
#include <unordered_map>
#include "SDL.h"
#include <Graphics_component.h>
#include "Settings.h"


namespace Game
{
	class Camera;


	namespace Graphics
	{
		class Render_instance;
		class Texture;


		class Renderer
		{
		public:
			explicit Renderer(const Core::Settings& settings, Core::Window& window);

			void clear(Color clear_color);
			void show();
			void set_render_scale(double x, double y);
			void render(const Resources::Texture_manager& tm, const Camera& camera) const;
			void render(const Render_instance& instance) const;
			void register_component(Objects::Graphics_component& gc);
			void remove_component(Objects::Graphics_component& gc);

		private:
			std::unordered_map<Geometry::Vector<int>, std::list<Objects::Graphics_component*>> grid_;
			const double grid_cell_size_ = 10.0; // Very temporary and random number for now
			const Core::Settings& settings_;
			std::unique_ptr<SDL_Renderer, Sdl_deleter> sdl_renderer_;

			friend class Texture;
		};


		class Aspect_ratio
		{
		public:
			Aspect_ratio(int x, int y);
			int x() const;
			int y() const;
			double x_multiplier() const;
			double y_multiplier() const;

		private:
			int x_, y_;
		};
	}
}

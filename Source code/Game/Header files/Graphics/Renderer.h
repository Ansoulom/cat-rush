#pragma once

#include <list>
#include <unordered_map>
#include "SDL.h"
#include <Graphics_component.h>


namespace Game
{
	class Camera;


	namespace Graphics
	{
		class Texture_manager;
		class Render_instance;


		class Renderer
		{
		public:
			Renderer();

			void render(const Texture_manager& tm, const Camera& camera, SDL_Renderer& sdl_renderer) const;
			void render(const Render_instance& instance, SDL_Renderer& sdl_renderer) const;
			void register_component(Objects::Graphics_component& gc);
			void remove_component(Objects::Graphics_component& gc);

		private:
			std::unordered_map<Geometry::Vector<int>, std::list<Objects::Graphics_component*>> grid_;
			const double grid_cell_size_ = 10.0; // Very temporary and random number for now
		};
	}
}

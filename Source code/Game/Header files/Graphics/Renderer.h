#pragma once

#include <list>
#include <unordered_map>
#include "SDL.h"
#include "Settings.h"
#include "Communication.h"
#include "Graphics_component.h"
#include "Resource_managers.h"


namespace Game
{
	namespace Objects
	{
		class Graphics_component;
	}


	namespace Resources
	{
		class Texture_manager;
	}


	class Camera;


	namespace Graphics
	{
		class Color;
		class Render_instance;
		class Texture;


		struct Render_settings
		{
			Geometry::Vector<int> normalized_to_pixels(Geometry::Vector<double> normalized) const;
			int normalized_to_pixels(double normalized) const;

			Geometry::Vector<int> render_size;
		};


		class Render_grid
		{
		public:
			Render_grid();

			std::vector<Render_instance> get_render_instances(const Resources::Texture_manager& tm, const Render_settings& render_settings, const Camera& camera) const;

			void register_component(Objects::Graphics_component& gc);
			void remove_component(Objects::Graphics_component& gc);
			void update_grid_position(Objects::Graphics_component& component, Geometry::Vector<double> old_position);

		private:
			void put_in_grid(Objects::Graphics_component& comp, Geometry::Vector<int> grid_cell_position);
			void remove_from_grid(Objects::Graphics_component& comp, Geometry::Vector<int> grid_cell_pos);

			static Geometry::Vector<int> get_grid_cell_position(Geometry::Vector<double> object_position);

			std::unordered_map<Geometry::Vector<int>, std::list<Objects::Graphics_component*>> grid_{};

			Communication::Receiver<Objects::Graphics_component&> component_destroyed_receiver_;
			Communication::Receiver<Objects::Graphics_component&, Geometry::Vector<double>> component_moved_receiver_;

			static const double grid_cell_size; // Very temporary and random number for now
		};


		class Renderer
		{
		public:
			explicit Renderer(const Core::Settings& settings, Core::Window& window);

			void clear(Color clear_color);
			void show();
			void set_render_scale(double x, double y);
			void render(const Render_instance& instance) const;
			Render_settings render_settings() const;

		private:
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


		std::vector<Render_instance> merge_render_lists(std::initializer_list<std::vector<Render_instance>> lists);
	}
}

#pragma once

#include "Shapes.h"
#include "Render_instance.h"
#include "Resource_managers.h"
#include "Renderer.h"
#include "Combat_component.h"


namespace Game {
	namespace Core {
		class World;
	}
}


namespace Game
{
	namespace Graphics
	{
		struct Render_settings;
	}
}


namespace Game
{
	namespace Gui
	{
		class Health_bar
		{
		public:
			Health_bar(Geometry::Rectangle<double> bounds, bool flipped);

			void set_max_health(int max_health);
			void set_health(int health);
			void connect_with_object(Objects::Combat_component& object);

			std::vector<Graphics::Render_instance> get_render_instances(
				const Resources::Texture_manager& tm,
				const Graphics::Render_settings& render_settings) const;

		private:
			int max_health_{1};
			int health_{max_health_};
			Geometry::Rectangle<double> bounds_;
			bool flipped_;
			Communication::Receiver<int> health_changed_receiver_;
		};


		class Hud
		{
		public:
			explicit Hud(Graphics::Aspect_ratio aspect_ratio);

			void connect_with_world(Core::World& world);

			std::vector<Graphics::Render_instance> get_render_instances(
				const Resources::Texture_manager& tm,
				const Graphics::Render_settings& render_settings);

		private:
			Graphics::Aspect_ratio aspect_ratio_;
			Health_bar player_bar_, boss_bar_;
		};
	}
}

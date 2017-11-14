#pragma once

#include "Component.h"
#include "Communication.h"
#include "Render_instance.h"
#include "Renderer.h"


namespace Game
{
	namespace Resources
	{
		class Texture_manager;
	}


	class Camera;


	namespace Graphics
	{
		class Renderer;
		struct Render_settings;
	}


	namespace Objects
	{
		class Game_object;


		class Graphics_component : public Component
		{
		public:
			explicit Graphics_component(Game_object& game_object);

			~Graphics_component() override;

			void receive(const Events::Message& message) override;

			virtual Graphics::Render_instance get_render_instance(
				const Resources::Texture_manager& tm,
				const Camera& camera,
				const Graphics::Render_settings& render_settings) const = 0;

			void add_destroy_receiver(Communication::Receiver<Graphics_component&>& receiver);
			void add_moved_receiver(Communication::Receiver<Graphics_component&, Geometry::Vector<double>>& receiver);

		private:
			template<typename T>
			void handle_event(const T& message) {}


			void handle_event(const Events::Position_changed& message);

			Communication::Dispatcher<Graphics_component&> destroy_event_;
			Communication::Dispatcher<Graphics_component&, Geometry::Vector<double>> moved_event_;
		};
	}
}

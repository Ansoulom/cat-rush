#pragma once

#include "Component.h"
#include "Communication.h"
#include "Render_instance.h"


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
	}


	namespace Objects
	{
		class Game_object;


		class Graphics_component : public Component
		{
		public:
			explicit Graphics_component(Game_object& game_object);
			virtual ~Graphics_component();

			void receive(const Events::Message& message) override;

			virtual Graphics::Render_instance get_render_instance(
				const Resources::Texture_manager& tm,
				const Camera& camera) const = 0;
			void add_destroy_listener(std::function<void(Graphics_component&)> function);

		private:
			Communication::Event<Graphics_component&> destroy_event_;
		};
	}
}

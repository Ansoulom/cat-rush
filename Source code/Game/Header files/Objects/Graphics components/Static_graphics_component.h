#pragma once

#include "Graphics_component.h"
#include <json.hpp>


namespace Game
{
	namespace Graphics
	{
		class Texture_manager;
	}


	namespace Objects
	{
		class Static_graphics_component :
			public Graphics_component
		{
		public:
			explicit Static_graphics_component(Game_object* container, std::string texture);
			virtual ~Static_graphics_component();

			//Static_graphics_component* clone() override;

			void update(Timer::Seconds time_passed) override;
			void receive(const Events::Message& message) override;
			Graphics::Render_instance get_render_instance(const Graphics::Texture_manager& tm, const Camera& camera) const
			override;

			static Static_graphics_component* from_json(const nlohmann::json& json, Game_object* game_object,
														const Component_loader& loader);

		private:
			nlohmann::json to_json() const override;

			std::string texture_name;;
		};
	}
}

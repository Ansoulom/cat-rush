#pragma once

#include "Graphics_component.h"
#include <json.hpp>


namespace Game
{
	namespace Resources
	{
		class Texture_manager;
	}


	namespace Objects
	{
		class Static_graphics_component : public Graphics_component
		{
		public:
			explicit Static_graphics_component(Game_object& game_object, std::string texture, int layer);
			virtual ~Static_graphics_component();

			void update(Timer::Seconds time_passed) override;
			void receive(const Events::Message& message) override;
			Graphics::Render_instance get_render_instance(const Resources::Texture_manager& tm, const Camera& camera) const
				override;

			static Static_graphics_component* from_json(const IO::json& json, Game_object& game_object);
			IO::json to_json() const override;

		private:
			std::string texture_name_;
			int layer_;

			static const Deserializer add_to_map;
		};
	}
}

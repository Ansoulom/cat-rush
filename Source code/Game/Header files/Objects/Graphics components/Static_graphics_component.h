#pragma once

#include "Graphics_component.h"
#include "JSON_extensions.h"


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
			Graphics::Render_instance get_render_instance(
					const Resources::Texture_manager& tm,
					const Camera& camera,
					const Graphics::Render_settings& render_settings) const
				override;

			static Static_graphics_component* from_json(
				const Io::json& json,
				Game_object& game_object,
				const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

		private:
			std::string texture_name_;
			int layer_;

			static const Deserializer add_to_map;
		};
	}
}

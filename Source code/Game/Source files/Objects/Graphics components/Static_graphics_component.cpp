#include "Static_graphics_component.h"
#include "Texture.h"
#include "Texture_manager.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game_object.h"
#include <Camera.h>
#include <Camera.h>

using nlohmann::json;


namespace Game
{
	namespace Objects
	{
		Static_graphics_component::
			Static_graphics_component(Game_object* container, std::string texture) : Graphics_component{container},
																					 texture_name_{texture} { }


		json Static_graphics_component::to_json() const
		{
			return {{"type", "static"}, {"texture", texture_name_}};
		}


		Static_graphics_component* Static_graphics_component::from_json(const json& json, Game_object* game_object,
																		const Component_loader& loader)
		{
			auto texture = json.at("texture").get<std::string>();

			auto component = new Static_graphics_component{game_object, texture};
			loader.register_component(*component);

			return component;
		}


		Static_graphics_component::~Static_graphics_component() { }

		void Static_graphics_component::update(Timer::Seconds time_passed) { }


		void Static_graphics_component::receive(const Events::Message& message)
		{
			Graphics_component::receive(message);
		}


		Graphics::Render_instance Static_graphics_component::get_render_instance(
			const Graphics::Texture_manager& tm, const Camera& camera) const
		{
			auto texture = tm.get_texture(texture_name_);
			return {
				texture.get(),
				{camera.get_coordinates_on_screen(game_object_->get_position()),texture->get_width(), texture->get_height()}
			};
		}
	}
}

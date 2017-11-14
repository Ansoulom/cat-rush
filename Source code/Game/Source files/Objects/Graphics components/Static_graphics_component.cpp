#include "Static_graphics_component.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game_object.h"
#include "Component.h"
#include "Resource_managers.h"


namespace Game
{
	namespace Objects
	{
		Static_graphics_component::
			Static_graphics_component(Game_object& game_object, std::string texture, int layer)
			: Graphics_component{game_object},
			  texture_name_{texture},
			  layer_{layer} { }


		Io::json Static_graphics_component::to_json() const
		{
			return {{"type", type()}, {"texture", texture_name_}, {"layer", layer_}};
		}


		std::string Static_graphics_component::component_type() const
		{
			return type();
		}


		std::string Static_graphics_component::type()
		{
			return "Static_graphics_component";
		}


		Static_graphics_component* Static_graphics_component::from_json(
			const Io::json& json,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			const auto texture = json.at("texture").get<std::string>();
			const auto layer = json.at("layer").get<int>();

			const auto component = new Static_graphics_component{game_object, texture, layer};

			return component;
		}


		Static_graphics_component::~Static_graphics_component() { }

		void Static_graphics_component::update(Timer::Seconds time_passed) { }


		void Static_graphics_component::receive(const Events::Message& message)
		{
			Graphics_component::receive(message);
		}


		Graphics::Render_instance Static_graphics_component::get_render_instance(
			const Resources::Texture_manager& tm,
			const Camera& camera,
			const Graphics::Render_settings& render_settings) const
		{
			const auto& texture = tm.get_texture(texture_name_);
			return {
				texture,
				{
					render_settings.normalized_to_pixels(camera.get_normalized_coordinates_on_screen(game_object().position())),
					texture.width(),
					texture.height()
				},
				layer_
			};
		}


		const Component::Deserializer Static_graphics_component::add_to_map{type(), from_json};
	}
}

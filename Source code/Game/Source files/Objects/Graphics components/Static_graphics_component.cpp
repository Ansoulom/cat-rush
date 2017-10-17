#include "Static_graphics_component.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game_object.h"
#include "Component.h"
#include "Resource_managers.h"
#include "World.h"


namespace Game
{
	namespace Objects
	{
		Static_graphics_component::
			Static_graphics_component(Game_object& game_object, std::string texture, int layer)
			: Graphics_component{game_object},
			  texture_name_{texture},
			  layer_{layer} { }


		IO::json Static_graphics_component::to_json() const
		{
			return {{"type", "Static_graphics_component"}, {"texture", texture_name_}, {"layer", layer_}};
		}


		Static_graphics_component* Static_graphics_component::from_json(const IO::json& json, Game_object& game_object)
		{
			const auto texture = json.at("texture").get<std::string>();
			const auto layer = json.at("layer").get<int>();

			const auto component = new Static_graphics_component{game_object, texture, layer};
			game_object.world().component_loader().register_component(*component);

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
			const Camera& camera) const
		{
			auto texture = tm.get_texture(texture_name_);
			return {texture.get(), camera.get_coordinates_on_screen(game_object().position()), layer_};
		}


		const Component::Deserializer Static_graphics_component::add_to_map{"Static_graphics_component", from_json};
	}
}

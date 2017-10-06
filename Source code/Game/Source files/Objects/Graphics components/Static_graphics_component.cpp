#include "Static_graphics_component.h"
#include "Texture.h"
#include "Texture_manager.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game_object.h"
#include "Component.h"


namespace Game
{
	namespace Objects
	{
		Static_graphics_component::
			Static_graphics_component(Game_object& game_object, std::string texture)
			: Graphics_component{game_object},
			  texture_name_{texture} { }


		IO::json Static_graphics_component::to_json() const
		{
			return {{"type", "static"}, {"texture", texture_name_}};
		}


		Static_graphics_component* Static_graphics_component::from_json(const IO::json& json, Game_object& game_object)
		{
			auto texture = json.at("texture").get<std::string>();

			auto component = new Static_graphics_component{game_object, texture};
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
			const Graphics::Texture_manager& tm,
			const Camera& camera) const
		{
			auto texture = tm.get_texture(texture_name_);
			return {texture.get(), camera.get_coordinates_on_screen(game_object_->get_position())};
		}
	}
}

#include "Animated_graphics_component.h"
#include "Renderer.h"
#include "Game_object.h"
#include "Camera.h"

using nlohmann::json;


namespace Game
{
	namespace Objects
	{
		Animated_graphics_component::Animated_graphics_component(Game_object* game_object,
																 std::unordered_map<std::string, Graphics::Animation> animations) : Graphics_component{game_object},
																																	animations{animations},
																																	current_animation{animations.size() > 0 ? animations.begin()->first : ""} { }


		Animated_graphics_component::~Animated_graphics_component() { }


		/*Animated_graphics_component* Animated_graphics_component::clone()
		{
			return new Animated_graphics_component{*this};
		}/**/

		void Animated_graphics_component::update(Timer::Seconds time_passed)
		{
			auto iter = animations.find(current_animation);
			if(iter != animations.end())
			{
				iter->second.update(time_passed);
			}
			// TODO: Decide if it should be okay for current_animation to be invalid or if exception should be thrown
		}


		void Animated_graphics_component::receive(const Events::Message& message)
		{
			Graphics_component::receive(message);
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		void Animated_graphics_component::set_row(int row)
		{
			auto iter = animations.find(current_animation);
			if(iter != animations.end())
			{
				iter->second.set_row(row);
			}
		}


		json Animated_graphics_component::to_json() const
		{
			auto animations_json = json{};
			for(const auto& animation_pair : animations)
			{
				animations_json.emplace_back(json{{"name", animation_pair.first}, {"animation", animation_pair.second.to_json()}});
			}
			return {{"type", "animated"}, {"animations", animations_json}};
		}


		void Animated_graphics_component::handle_event(const Events::Rotation_changed& message)
		{
			assert(message.new_rotation >= 0.0 && message.new_rotation < 360.0);
			set_row(static_cast<int>(round(message.new_rotation / 45.0)) % 8); // Note: May not be safe for all animations
		}


		Animated_graphics_component* Animated_graphics_component::from_json(const json& json, Game_object* game_object,
																			const Component_loader& loader)
		{
			auto animations = std::unordered_map<std::string, Graphics::Animation>{};
			for(auto animation : json.at("animations"))
			{
				animations.emplace(animation.at("name").get<std::string>(),
								   Graphics::Animation::from_json(animation.at("animation")));
			}

			auto component = new Animated_graphics_component{game_object, animations};
			loader.register_component(*component);

			return component;
		}


		Graphics::Render_instance Animated_graphics_component::get_render_instance(
			const Graphics::Texture_manager& tm, const Camera& camera) const
		{
			auto frame = animations.at(current_animation).get_current_frame(tm);
			auto clip = frame.get_clip();
			return {
				&frame.get_texture(), {camera.get_coordinates_on_screen(game_object->get_position()), clip.width(), clip.height()},
				&clip
			};
		}
	}
}

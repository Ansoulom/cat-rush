#include "Animated_graphics_component.h"
#include "Renderer.h"
#include "Game_object.h"
#include "Camera.h"
#include "Component.h"
#include "World.h"


namespace Game
{
	namespace Objects
	{
		Animated_graphics_component::Animated_graphics_component(
			Game_object& game_object,
			int layer,
			std::unordered_map<std::string, Graphics::Animation> animations)
			: Graphics_component{game_object},
			  animations_{animations},
			  current_animation_{animations.size() > 0 ? animations.begin()->first : ""},
			  flipped_{false},
			  layer_{layer} { }


		void Animated_graphics_component::update(Timer::Seconds time_passed)
		{
			auto iter = animations_.find(current_animation_);
			if(iter != animations_.end())
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
			auto iter = animations_.find(current_animation_);
			if(iter != animations_.end())
			{
				iter->second.set_row(row);
			}
		}


		IO::json Animated_graphics_component::to_json() const
		{
			auto animations_json = IO::json{};
			for(const auto& animation_pair : animations_)
			{
				animations_json.emplace_back(
					IO::json{{"name", animation_pair.first}, {"animation", animation_pair.second.to_json()}});
			}
			return {{"type", "Animated_graphics_component"}, {"animations", animations_json}, {"layer", layer_}};
		}


		void Animated_graphics_component::handle_event(const Events::Direction_changed& message)
		{
			flipped_ = message.direction == Direction::left;
		}


		Animated_graphics_component* Animated_graphics_component::from_json(
			const IO::json& json,
			Game_object& game_object)
		{
			auto animations = std::unordered_map<std::string, Graphics::Animation>{};
			for(auto animation : json.at("animations"))
			{
				animations.emplace(
					animation.at("name").get<std::string>(),
					Graphics::Animation::from_json(animation.at("animation")));
			}
			const auto layer = json.at("layer").get<int>();

			const auto component = new Animated_graphics_component{game_object, layer, animations};
			game_object.world().component_loader().register_component(*component);

			return component;
		}


		Graphics::Render_instance Animated_graphics_component::get_render_instance(
			const Resources::Texture_manager& tm,
			const Camera& camera) const
		{
			auto frame = animations_.at(current_animation_).get_current_frame(tm);
			auto clip = frame.get_clip();
			return Graphics::Render_instance{
				&frame.get_texture(),
				camera.get_coordinates_on_screen(game_object().position()),
				layer_,
				clip,
				{},
				{},
				flipped_
			};
		}


		const Component::Deserializer Animated_graphics_component::add_to_map{
			"Animated_graphics_component",
			from_json
		};
	}
}

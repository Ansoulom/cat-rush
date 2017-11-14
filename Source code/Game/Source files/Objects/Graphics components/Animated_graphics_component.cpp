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
				if(iter->second.update(time_passed))
				{
					game_object().send(Events::Animation_finished{current_animation_});
				}
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


		Io::json Animated_graphics_component::to_json() const
		{
			auto animations_json = Io::json{};
			for(const auto& animation_pair : animations_)
			{
				animations_json.emplace_back(
					Io::json{{"name", animation_pair.first}, {"animation", animation_pair.second.to_json()}});
			}
			return {{"type", type()}, {"animations", animations_json}, {"layer", layer_}};
		}


		std::string Animated_graphics_component::component_type() const
		{
			return type();
		}


		std::string Animated_graphics_component::type()
		{
			return "Animated_graphics_component";
		}


		void Animated_graphics_component::handle_event(const Events::State_changed& message)
		{
			const auto iter = animations_.find(message.state);
			if(iter != animations_.end())
			{
				current_animation_ = message.state;
				iter->second.reset();
			}
		}


		void Animated_graphics_component::handle_event(const Events::Direction_changed& message)
		{
			flipped_ = message.direction == Direction_x::left;
		}


		Animated_graphics_component* Animated_graphics_component::from_json(
			const Io::json& json,
			Game_object& game_object,
			const Component_type_map& component_map)
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

			return component;
		}


		Graphics::Render_instance Animated_graphics_component::get_render_instance(
			const Resources::Texture_manager& tm,
			const Camera& camera,
			const Graphics::Render_settings& render_settings) const
		{
			auto frame = animations_.at(current_animation_).current_frame(tm);
			auto clip = frame.clip();

			return {
				frame.texture(),
				{
					render_settings.normalized_to_pixels(camera.get_normalized_coordinates_on_screen(game_object().position())),
					clip.width(),
					clip.height()
				},
				layer_,
				clip,
				flipped_,
				false
			};
		}


		const Component::Deserializer Animated_graphics_component::add_to_map{
			type(),
			from_json
		};
	}
}

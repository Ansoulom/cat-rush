#pragma once

#include "Animation.h"
#include "Graphics_component.h"
#include <unordered_map>
#include "Timer.h"
#include "json.hpp"


namespace Game
{
	namespace Objects
	{
		class Animated_graphics_component : public Graphics_component
		{
		public:
			explicit Animated_graphics_component(
				Game_object& game_object,
				int layer,
				std::unordered_map<std::string, Graphics::Animation> animations = {}
			);
			// TODO: Make it possible to change active animation and make sure to avoid problems with empty map arguments

			void update(Timer::Seconds time_passed) override; // The object passed is the owner of the component.
			void receive(const Events::Message& message) override;
			Graphics::Render_instance get_render_instance(
				const Resources::Texture_manager& tm,
				const Camera& camera) const override;
			void set_row(int row); // The row of sprites, e.g. direction or state. Must be valid for the current animation.

			static Animated_graphics_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

		private:
			template<typename T>
			void handle_event(const T& message) {}


			void handle_event(const Events::Direction_changed& message);

			std::unordered_map<std::string, Graphics::Animation> animations_;
			std::string current_animation_;
			bool flipped_;
			int layer_;

			static const Deserializer add_to_map;
		};
	}
}

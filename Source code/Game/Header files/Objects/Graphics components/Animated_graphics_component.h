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
		class Animated_graphics_component :
			public Graphics_component
		{
		public:
			explicit Animated_graphics_component(Game_object& game_object, std::unordered_map<std::string, Graphics::Animation> animations = {}); // TODO: Make it possible to change active animation and make sure to avoid problems with empty map arguments
			virtual ~Animated_graphics_component();

			void update(Timer::Seconds time_passed) override; // The object passed is the owner of the component.
			void receive(const Events::Message& message) override;
			Graphics::Render_instance get_render_instance(const Graphics::Texture_manager& tm, const Camera& camera) const override;
			void set_row(int row); // The row of sprites, e.g. direction or state. Must be valid for the current animation.

			static Animated_graphics_component* from_json(const IO::json& json, Game_object& game_object);
			IO::json to_json() const override;

		private:
			template<typename T>
			void handle_event(const T& message){}
			void handle_event(const Events::Rotation_changed& message);

			std::unordered_map<std::string, Graphics::Animation> animations_;
			std::string current_animation_;
			
			static const Deserializer add_to_map;
		};
	}
}

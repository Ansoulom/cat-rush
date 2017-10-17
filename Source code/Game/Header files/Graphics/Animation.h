#pragma once

#include "Timer.h"
#include <json.hpp>
#include <Texture.h>
#include <Shapes.h>


namespace Game
{
	namespace Resources
	{
		class Texture_manager;
	}
}


namespace Game
{
	namespace Graphics
	{
		class Animation
		{
		public:
			class Frame
			{
			public:
				Frame(Texture& texture, const Geometry::Rectangle<int>& clip);

				Texture& get_texture() const;
				Geometry::Rectangle<int> get_clip() const;

			private:
				Texture& texture_;
				Geometry::Rectangle<int> clip_;
			};


			Animation(std::string texture, int frames, int rows, double frame_rate);
			// TODO: Document and handle class invariants and consider default constructor
			~Animation();

			Frame get_current_frame(const Resources::Texture_manager& tm) const;
			void update(Timer::Seconds time_passed);
			void set_row(int row); // The row of sprites, e.g. direction or state.

			static Animation from_json(const nlohmann::json& json);
			Io::json to_json() const;

		private:
			std::string texture_name_;
			int frames_;
			int rows_;
			int current_frame_;
			int current_row_;
			double frame_rate_;
			Timer timer_;
		};
	}
}

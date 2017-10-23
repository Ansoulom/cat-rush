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
				Frame(const Texture& texture, const Geometry::Rectangle<int>& clip);

				const Texture& texture() const;
				Geometry::Rectangle<int> clip() const;

			private:
				const Texture* texture_;
				Geometry::Rectangle<int> clip_;
			};

			// TODO: Document and handle class invariants and consider default constructor
			Animation(std::string texture, int frames, int rows, double frame_rate, bool looping);
			

			Frame current_frame(const Resources::Texture_manager& tm) const;

			// Returns true if animation just finished, false otherwise.
			bool update(Timer::Seconds time_passed);
			// The row of sprites, e.g. direction or state.
			void set_row(int row);
			void reset();

			static Animation from_json(const Io::json& json);
			Io::json to_json() const;

		private:
			std::string texture_name_;
			int frames_;
			int rows_;
			int current_frame_;
			int current_row_;
			double frame_rate_;
			Timer timer_;
			bool looping_;
			bool finished_{};
		};
	}
}

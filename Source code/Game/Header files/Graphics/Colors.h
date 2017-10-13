#pragma once
#include <cstdint>
#include <limits>


namespace Game
{
	namespace Graphics
	{
		class Color
		{
		public:
			Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = std::numeric_limits<uint8_t>::max());

			uint8_t& red();
			uint8_t red() const;
			uint8_t& green();
			uint8_t green() const;
			uint8_t& blue();
			uint8_t blue() const;
			uint8_t& alpha();
			uint8_t alpha() const;

		private:
			uint8_t r_, g_, b_, a_;
		};
	}
}

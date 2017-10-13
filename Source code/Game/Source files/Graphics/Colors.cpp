#include "Colors.h"


namespace Game
{
	namespace Graphics
	{
		Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r_{red}, g_{green}, b_{blue}, a_{alpha} { }


		uint8_t& Color::red()
		{
			return r_;
		}


		uint8_t Color::red() const
		{
			return r_;
		}


		uint8_t& Color::green()
		{
			return g_;
		}


		uint8_t Color::green() const
		{
			return g_;
		}


		uint8_t& Color::blue()
		{
			return b_;
		}


		uint8_t Color::blue() const
		{
			return b_;
		}


		uint8_t& Color::alpha()
		{
			return a_;
		}


		uint8_t Color::alpha() const
		{
			return a_;
		}
	}
}

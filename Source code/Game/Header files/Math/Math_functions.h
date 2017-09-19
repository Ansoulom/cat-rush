#pragma once

#include <cmath>

namespace Game
{
	namespace Math
	{
		const double pi = 3.14159265358979323846;


		template<typename T>
		int sign(T val)
		{
			return (T(0) < val) - (val < T(0));
		}


		template<typename T>
		auto rad_to_deg(T val) -> decltype(val * 180.0 / pi)
		{
			return val * 180.0 / pi;
		}


		template<typename T>
		auto deg_to_rad(T val) -> decltype(val * pi / 180.0)
		{
			return val * pi / 180.0;
		}


		template<typename T>
		auto clamp_angle(T angle) -> decltype(fmod(angle, 360))
		{
			angle = std::fmod(angle, 360);
			if (angle < 0)
			{
				angle += 360;
			}
			if(angle == 360)
			{
				angle = 0;
			}
			assert(angle >= 0);
			assert(angle < 360);
			return angle;
		}
	}
}

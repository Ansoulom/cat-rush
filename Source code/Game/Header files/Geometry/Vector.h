#pragma once

#include <cmath>
#include <functional>
#include "Math_functions.h"


namespace Game
{
	namespace Geometry
	{
		template<typename T>
		class Vector
		{
		public:
			Vector();
			Vector(T x, T y);
			~Vector();

			Vector& operator+=(const Vector& other);
			Vector& operator-=(const Vector& other);
			Vector& operator*=(T other);
			Vector& operator/=(T other);

			T get_magnitude() const; // Returns the length.
			T get_angle() const; // Returns the angle in degrees, from the positive x axis towards the positive y axis.
			T& x();
			T& y();
			T get_x() const;
			T get_y() const;
			void set_x(T x);
			void set_y(T y);
			void set_magnitude(T magnitude); // Sets the length of the vector.
			void normalize(); // Set the length to a value between 0 and 1.
		private:
			T x_, y_;
		};


		template<typename T>
		Vector<T>::Vector() : x_{}, y_{}
		{
		}


		template<typename T>
		Vector<T>::Vector(T x, T y) : x_{ x }, y_{ y }
		{
		}


		template<typename T>
		Vector<T>::~Vector()
		{
		}


		template<typename T>
		Vector<T>& Vector<T>::operator+=(const Vector<T>& other)
		{
			x_ += other.x_;
			y_ += other.y_;
			return *this;
		}


		template<typename T>
		Vector<T>& Vector<T>::operator-=(const Vector<T>& other)
		{
			x_ -= other.x_;
			y_ -= other.y_;
			return *this;
		}


		template<typename T>
		Vector<T>& Vector<T>::operator*=(T other)
		{
			x_ *= other;
			y_ *= other;
			return *this;
		}


		template<typename T>
		Vector<T>& Vector<T>::operator/=(T other)
		{
			x_ /= other;
			y_ /= other;
			return *this;
		}



		template<typename T>
		T Vector<T>::get_magnitude() const
		{
			return sqrt(x_ * x_ + y_ * y_);
		}


		template<typename T>
		T Vector<T>::get_angle() const
		{
			return Math::rad_to_deg(atan2(y_, x_));
		}


		template<typename T>
		T& Vector<T>::x()
		{
			return x_;
		}


		template<typename T>
		T& Vector<T>::y()
		{
			return y_;
		}


		template<typename T>
		T Vector<T>::get_x() const
		{
			return x_;
		}


		template<typename T>
		T Vector<T>::get_y() const
		{
			return y_;
		}


		template<typename T>
		void Vector<T>::set_x(T x)
		{
			this->x_ = x;
		}


		template<typename T>
		void Vector<T>::set_y(T y)
		{
			this->y_ = y;
		}


		template<typename T>
		void Vector<T>::set_magnitude(T magnitude)
		{
			auto rad = Math::deg_to_rad(get_angle());
			x_ = cos(rad) * magnitude;
			y_ = sin(rad) * magnitude;
		}


		template<typename T>
		void Vector<T>::normalize()
		{
			if (get_magnitude() > 1.0) set_magnitude(1.0);
		}


		template<typename T>
		Vector<T> operator+(Vector<T> left, const Vector<T>& right)
		{
			left += right;
			return left;
		}


		template<typename T>
		Vector<T> operator-(Vector<T> left, const Vector<T>& right)
		{
			left -= right;
			return left;
		}


		template<typename T>
		Vector<T> operator*(Vector<T> left, T right)
		{
			left *= right;
			return left;
		}


		template<typename T>
		Vector<T> operator*(T left, Vector<T> right)
		{
			return right * left;
		}


		template<typename T>
		Vector<T> operator/(Vector<T> left, T right)
		{
			left /= right;
			return left;
		}


		template<typename T>
		bool operator==(Vector<T> left, Vector<T> right)
		{
			return left.get_x() == right.get_x() && left.get_y() == right.get_y(); // Floating point comparisons?
		}


		template<typename T>
		bool operator!=(Vector<T> left, Vector<T> right)
		{
			return left.get_x() != right.get_x() || left.get_y() != right.get_y(); // -||-
		}
	}
}


namespace std
{
	template<typename T>
	struct hash<Game::Geometry::Vector<T>>
	{
		size_t operator()(const Game::Geometry::Vector<T>& vec) const
		{
			// I hate hash functions
			auto lhs = vec.get_x();
			auto rhs = vec.get_y();
			lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
			return lhs;
		}
	};
}
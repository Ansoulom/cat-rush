#pragma once

#include "Vector.h"
#include "json.hpp"


namespace Game
{
	namespace Geometry
	{
		enum class Pivot_point
		{
			center,
			top_left,
			top_center,
			top_right,
			center_right,
			bottom_right,
			bottom_center,
			bottom_left,
			center_left
		};


		template<typename T>
		class Rectangle;
		template<typename T>
		class Circle;


		template<typename T>
		class Shape
		{
		public:
			virtual ~Shape();

			virtual void set_position(Vector<T> position) = 0;

			virtual bool intersects(const Shape& other) const = 0;
			virtual bool intersects(const Circle<T>& other) const = 0;
			virtual bool intersects(const Rectangle<T>& other) const = 0;

			virtual T x_intersection(const Shape& other) const = 0;
			virtual T x_intersection(const Circle<T>& other) const = 0;
			virtual T x_intersection(const Rectangle<T>& other) const = 0;

			virtual T y_intersection(const Shape& other) const = 0;
			virtual T y_intersection(const Circle<T>& other) const = 0;
			virtual T y_intersection(const Rectangle<T>& other) const = 0;

			static Shape* from_json(const IO::json& j);
			virtual IO::json to_json() const = 0;
		};


		template<typename T>
		class Rectangle : public Shape<T>
		{
		public:
			Rectangle(Vector<T> position, T width, T height, Pivot_point pivot = Pivot_point::center);

			Vector<T> center() const;
			T width() const;
			T height() const;
			T right() const;
			T left() const;
			T top() const;
			T bottom() const;
			void set_position(Vector<T> position) override;
			void set_position(Vector<T> position, Pivot_point pivot);
			void set_width(T width);
			void set_height(T height);

			bool intersects(const Shape<T>& other) const override;
			bool intersects(const Rectangle& other) const override;
			bool intersects(const Circle<T>& other) const override;

			T x_intersection(const Shape<T>& other) const override;
			T x_intersection(const Rectangle& other) const override;
			T x_intersection(const Circle<T>& other) const override;

			T y_intersection(const Shape<T>& other) const override;
			T y_intersection(const Rectangle& other) const override;
			T y_intersection(const Circle<T>& other) const override;

			static Rectangle* from_json(const IO::json& j);
			IO::json to_json() const override;

		private:
			void correct_center(Pivot_point pivot);

			Vector<T> pos;
			T w, h;
		};


		template<typename T>
		class Circle : public Shape<T>
		{
		public:
			Circle(Vector<T> position, T radius);

			Vector<T> center() const;
			T radius() const;
			void set_position(Vector<T> position) override;
			void set_radius(T radius);

			bool intersects(const Shape<T>& other) const override;
			bool intersects(const Rectangle<T>& other) const override;
			bool intersects(const Circle& other) const override;

			T x_intersection(const Shape<T>& other) const override;
			T x_intersection(const Rectangle<T>& other) const override;
			T x_intersection(const Circle& other) const override;

			T y_intersection(const Shape<T>& other) const override;
			T y_intersection(const Rectangle<T>& other) const override;
			T y_intersection(const Circle& other) const override;

			static Circle* from_json(const IO::json& j);
			IO::json to_json() const override;

		private:
			Vector<T> pos;
			T r;
		};


		template<typename T>
		bool intersects(const Shape<T>& first, const Shape<T>& second)
		{
			return first.intersects(second);
		}


		template<typename T>
		bool intersects(const Rectangle<T>& first, const Rectangle<T>& second)
		{
			if(first.left() >= second.right())
			{
				return false;
			}
			if(first.right() <= second.left())
			{
				return false;
			}
			if(first.top() >= second.bottom())
			{
				return false;
			}
			if(first.bottom() <= second.top())
			{
				return false;
			}
			return true;
		}


		template<typename T>
		bool intersects(const Rectangle<T>& first, const Circle<T>& second)
		{
			auto closest_x = second.center().get_x() < first.left() ? first.left() : (second.center().get_x() > first.right() ? first.right() : second.center().get_x());
			auto closest_y = second.center().get_y() < first.top() ? first.top() : (second.center().get_y() > first.bottom() ? first.bottom() : second.center().get_y());

			auto distance_squared = pow(closest_x - second.center().get_x(), 2.0) + pow(closest_y - second.center().get_y(), 2.0);
			auto total_radius_squared = pow(second.radius(), 2.0);

			return distance_squared < total_radius_squared;
		}


		template<typename T>
		bool intersects(const Circle<T>& first, const Circle<T>& second)
		{
			auto distance_squared = pow(second.center().get_x() - first.center().get_x(), 2.0) + pow(second.center().get_y() - first.center().get_y(), 2.0);
			auto total_radius_squared = pow(first.radius() + second.radius(), 2.0);

			return distance_squared < total_radius_squared;
		}


		template<typename T>
		bool intersects(const Circle<T>& first, const Rectangle<T>& second)
		{
			return intersects(second, first);
		}


		template<typename T>
		Vector<T> intersection(const Shape<T>& first, const Shape<T>& second, Vector<double> direction)
		{
			//assert(direction.get_magnitude() == T{ 1 }); // Floating point comparison
			return { x_intersection(first, second) * direction.get_x(), y_intersection(first, second) * direction.get_y()}; // TODO: Implement non-lazy and optimized solution
		}


		template<typename T>
		T x_intersection(const Shape<T>& first, const Shape<T>& second)
		{
			return first.x_intersection(second);
		}


		template<typename T>
		T x_intersection(const Rectangle<T>& first, const Rectangle<T>& second)
		{
			if(!intersects(first, second)) return T{};

			if(first.left() < second.left()) return first.right() - second.left();
			return first.left() - second.right();
		}


		template<typename T>
		T x_intersection(const Rectangle<T>& first, const Circle<T>& second)
		{
			auto closest_x = second.center().get_x() < first.left() ? first.left() : (second.center().get_x() > first.right() ? first.right() : second.center().get_x());
			auto closest_y = second.center().get_y() < first.top() ? first.top() : (second.center().get_y() > first.bottom() ? first.bottom() : second.center().get_y());

			auto distance_squared = pow(closest_x - second.center().get_x(), 2.0) + pow(closest_y - second.center().get_y(), 2.0);
			auto total_radius_squared = pow(second.radius(), 2.0);

			if(distance_squared < total_radius_squared)
			{
				return static_cast<T>(second.center().get_x() > closest_x ? -(second.center().get_x() - closest_x - sqrt(total_radius_squared - pow(closest_y - second.center().get_y(), 2.0))) : -(second.center().get_x() - closest_x + sqrt(total_radius_squared - pow(closest_y - second.center().get_y(), 2.0))));
			}

			return T{};
		}


		template<typename T>
		T x_intersection(const Circle<T>& first, const Circle<T>& second)
		{
			auto distance_squared = pow(second.center().get_x() - first.center().get_x(), 2.0) + pow(second.center().get_y() - first.center().get_y(), 2.0);
			auto total_radius_squared = pow(first.radius() + second.radius(), 2.0);

			if(distance_squared < total_radius_squared)
			{
				return static_cast<T>(first.center().get_x() > second.center().get_x() ? first.center().get_x() - second.center().get_x() - sqrt(total_radius_squared - pow(second.center().get_y() - first.center().get_y(), 2.0)) : first.center().get_x() - second.center().get_x() + sqrt(total_radius_squared - pow(second.center().get_y() - first.center().get_y(), 2.0)));
			}

			return T{};
		}


		template<typename T>
		T x_intersection(const Circle<T>& first, const Rectangle<T>& second)
		{
			return -x_intersection(second, first);
		}


		template<typename T>
		T y_intersection(const Shape<T>& first, const Shape<T>& second)
		{
			return first.y_intersection(second);
		}


		template<typename T>
		T y_intersection(const Rectangle<T>& first, const Rectangle<T>& second)
		{
			if(!intersects(first, second)) return T{};

			if(first.top() < second.top()) return first.bottom() - second.top();
			return first.top() - second.bottom();
		}


		template<typename T>
		T y_intersection(const Rectangle<T>& first, const Circle<T>& second)
		{
			auto closest_x = second.center().get_x() < first.left() ? first.left() : (second.center().get_x() > first.right() ? first.right() : second.center().get_x());
			auto closest_y = second.center().get_y() < first.top() ? first.top() : (second.center().get_y() > first.bottom() ? first.bottom() : second.center().get_y());

			auto distance_squared = pow(closest_x - second.center().get_x(), 2.0) + pow(closest_y - second.center().get_y(), 2.0);
			auto total_radius_squared = pow(second.radius(), 2.0);

			if(distance_squared < total_radius_squared)
			{
				return static_cast<T>(second.center().get_y() > closest_y ? -(second.center().get_y() - closest_y - sqrt(total_radius_squared - pow(closest_x - second.center().get_x(), 2.0))) : -(second.center().get_y() - closest_y + sqrt(total_radius_squared - pow(closest_x - second.center().get_x(), 2.0))));
			}

			return T{};
		}


		template<typename T>
		T y_intersection(const Circle<T>& first, const Circle<T>& second)
		{
			auto distance_squared = pow(second.center().get_x() - first.center().get_x(), 2.0) + pow(second.center().get_y() - first.center().get_y(), 2.0);
			auto total_radius_squared = pow(first.radius() + second.radius(), 2.0);

			if(distance_squared < total_radius_squared)
			{
				return static_cast<T>(first.center().get_y() > second.center().get_y() ? first.center().get_y() - second.center().get_y() - sqrt(total_radius_squared - pow(second.center().get_x() - first.center().get_x(), 2.0)) : first.center().get_y() - second.center().get_y() + sqrt(total_radius_squared - pow(second.center().get_x() - first.center().get_x(), 2.0)));
			}

			return T{};
		}


		template<typename T>
		T y_intersection(const Circle<T>& first, const Rectangle<T>& second)
		{
			return -y_intersection(second, first);
		}


		template<typename T>
		bool intersects(const Rectangle<T>& rect, const Vector<T>& point)
		{
			return point.get_x() > rect.left() && point.get_x() < rect.right() && point.get_y() > rect.top && point.get_y() < rect.bottom();
		}


		template<typename T>
		Shape<T>::~Shape()
		{
		}


		template<typename T>
		Shape<T>* Shape<T>::from_json(const IO::json& j)
		{
			auto type = j.at("type").get<std::string>();
			if(type == "Rectangle")
			{
				return Rectangle<T>::from_json(j);
			}
			if(type == "Circle")
			{
				return Circle<T>::from_json(j);
			}
			throw std::runtime_error{"Incorrect shape type in json file"};
		}


		template<typename T>
		Rectangle<T>::Rectangle(Vector<T> center_position, T width, T height, Pivot_point pivot) : pos{center_position}, w{width}, h{height}
		{
			correct_center(pivot);
		}


		template<typename T>
		Vector<T> Rectangle<T>::center() const
		{
			return pos;
		}


		template<typename T>
		T Rectangle<T>::width() const
		{
			return w;
		}


		template<typename T>
		T Rectangle<T>::height() const
		{
			return h;
		}


		template<typename T>
		T Rectangle<T>::right() const
		{
			return pos.get_x() + w / 2;
		}


		template<typename T>
		T Rectangle<T>::left() const
		{
			return pos.get_x() - w / 2;
		}


		template<typename T>
		T Rectangle<T>::top() const
		{
			return pos.get_y() - h / 2;
		}


		template<typename T>
		T Rectangle<T>::bottom() const
		{
			return pos.get_y() + h / 2;
		}


		template<typename T>
		void Rectangle<T>::set_position(Vector<T> position)
		{
			pos = position;
		}


		template<typename T>
		void Rectangle<T>::set_position(Vector<T> position, Pivot_point pivot)
		{
			pos = position;
			correct_center(pivot);
		}


		template<typename T>
		void Rectangle<T>::set_width(T width)
		{
			w = width;
		}


		template<typename T>
		void Rectangle<T>::set_height(T height)
		{
			h = height;
		}


		template<typename T>
		bool Rectangle<T>::intersects(const Shape<T>& other) const
		{
			return other.intersects(*this);
		}


		template<typename T>
		bool Rectangle<T>::intersects(const Rectangle& other) const
		{
			return Geometry::intersects(*this, other);
		}


		template<typename T>
		bool Rectangle<T>::intersects(const Circle<T>& other) const
		{
			return Geometry::intersects(*this, other);
		}


		template<typename T>
		T Rectangle<T>::x_intersection(const Shape<T>& other) const
		{
			return -other.x_intersection(*this);
		}


		template<typename T>
		T Rectangle<T>::x_intersection(const Rectangle& other) const
		{
			return Geometry::x_intersection(*this, other);
		}


		template<typename T>
		T Rectangle<T>::x_intersection(const Circle<T>& other) const
		{
			return Geometry::x_intersection(*this, other);
		}


		template<typename T>
		T Rectangle<T>::y_intersection(const Shape<T>& other) const
		{
			return -other.y_intersection(*this);
		}


		template<typename T>
		T Rectangle<T>::y_intersection(const Rectangle& other) const
		{
			return Geometry::y_intersection(*this, other);
		}


		template<typename T>
		T Rectangle<T>::y_intersection(const Circle<T>& other) const
		{
			return Geometry::y_intersection(*this, other);
		}


		template<typename T>
		Rectangle<T>* Rectangle<T>::from_json(const IO::json& j)
		{
			return new Rectangle{{j.at("x_position").get<T>(), j.at("y_position").get<T>()}, j.at("width").get<T>(), j.at("height").get<T>()};
		}


		template<typename T>
		IO::json Rectangle<T>::to_json() const
		{
			return IO::json{{"type", "rectangle"}, {"x_position", pos.get_x()}, {"y_position", pos.get_y()}, {"width", w}, {"height", h}};
		}


		template<typename T>
		void Rectangle<T>::correct_center(Pivot_point pivot)
		{
			switch(pivot)
			{
				case Pivot_point::center:
					return;
				case Pivot_point::top_left:
					pos += Vector<T>{w / 2, h / 2};
					break;
				case Pivot_point::top_center:
					pos += Vector<T>{0, h / 2};
					break;
				case Pivot_point::top_right:
					pos += Vector<T>{-w / 2, h / 2};
					break;
				case Pivot_point::center_right:
					pos += Vector<T>{-w / 2, 0};
					break;
				case Pivot_point::bottom_right:
					pos += Vector<T>{-w / 2, -h / 2};
					break;
				case Pivot_point::bottom_center:
					pos += Vector<T>{0, -h / 2};
					break;
				case Pivot_point::bottom_left:
					pos += Vector<T>{w / 2, -h / 2};
					break;
				case Pivot_point::center_left:
					pos += Vector<T>{w / 2, 0};
					break;
			}
		}


		template<typename T>
		Circle<T>::Circle(Vector<T> position, T radius) : pos{position}, r{radius}
		{
		}


		template<typename T>
		Vector<T> Circle<T>::center() const
		{
			return pos;
		}


		template<typename T>
		T Circle<T>::radius() const
		{
			return r;
		}


		template<typename T>
		void Circle<T>::set_position(Vector<T> position)
		{
			pos = position;
		}


		template<typename T>
		void Circle<T>::set_radius(T radius)
		{
			r = radius;
		}


		template<typename T>
		bool Circle<T>::intersects(const Shape<T>& other) const
		{
			return other.intersects(*this);
		}


		template<typename T>
		bool Circle<T>::intersects(const Rectangle<T>& other) const
		{
			return Geometry::intersects(*this, other);
		}


		template<typename T>
		bool Circle<T>::intersects(const Circle& other) const
		{
			return Geometry::intersects(*this, other);
		}


		template<typename T>
		T Circle<T>::x_intersection(const Shape<T>& other) const
		{
			return -other.x_intersection(*this);
		}


		template<typename T>
		T Circle<T>::x_intersection(const Rectangle<T>& other) const
		{
			return Geometry::x_intersection(*this, other);
		}


		template<typename T>
		T Circle<T>::x_intersection(const Circle& other) const
		{
			return Geometry::x_intersection(*this, other);
		}


		template<typename T>
		T Circle<T>::y_intersection(const Shape<T>& other) const
		{
			return -other.y_intersection(*this);
		}


		template<typename T>
		T Circle<T>::y_intersection(const Rectangle<T>& other) const
		{
			return Geometry::y_intersection(*this, other);
		}


		template<typename T>
		T Circle<T>::y_intersection(const Circle& other) const
		{
			return Geometry::y_intersection(*this, other);
		}


		template<typename T>
		Circle<T>* Circle<T>::from_json(const IO::json& j)
		{
			return new Circle{{j.at("x_position").get<T>(), j.at("y_position").get<T>()}, j.at("radius").get<T>()};
		}


		template<typename T>
		IO::json Circle<T>::to_json() const
		{
			return IO::json{{"type", "circle"},{"x_position", pos.get_x()},{"y_position", pos.get_y()},{"radius", r}};
		}
	}
}

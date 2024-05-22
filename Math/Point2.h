#ifndef _THEIA_MATH_POINT2_H_
#define _THEIA_MATH_POINT2_H_
#include "../Types.h"
#include "Vector2.h"
#include <assert.h>

namespace Theia {
	template<typename T> class Point2 {
	public:
		Point2() :
			m_x(T{}),
			m_y(T{})
		{

		}

		Point2(T x, T y) :
			m_x(x),
			m_y(y)
		{

		}

		Point2(const Point2& point) :
			m_x(point.m_x),
			m_y(point.m_y)
		{

		}

		template <typename U> explicit Point2(const Point2<U>& point) :
			m_x(T(point.m_x)),
			m_y(T(point.m_y))
		{

		}

		template <typename U> explicit Point2(U x, U y) :
			m_x(T(x)),
			m_y(T(y))
		{

		}

		Point2& operator=(const Point2& point) {
			m_x = point.m_x;
			m_y = point.m_y;

			return *this;
		}

		bool operator==(const Point2& point) const {
			return m_x == point.m_x && m_y == point.m_y;
		}

		bool operator!=(const Point2& point) const {
			return m_x != point.m_x || m_y != point.m_y;
		}

		T operator[](uint32_t index) const {
			assert(index < 2, "Point2 access index out of bounds.");
			if (index == 0) {
				return m_x;
			}
			else {
				return m_y;
			}
		}

		T& operator[](uint32_t index) {
			assert(index < 2, "Point2 access index out of bounds.");
			if (index == 0) {
				return m_x;
			}
			else {
				return m_y;
			}
		}

		Vector2<T> operator-(const Point2& point) const {
			return Vector2<T>(m_x - point.m_x, m_y - point.m_y);
		}

		Point2 operator+(const Vector2<T>& vector) const {
			return Point2(m_x + vector.m_x, m_y + vector.m_y);
		}

		Point2 operator-(const Vector2<T>& vector) const {
			return Point2(m_x - vector.m_x, m_y - vector.m_y);
		}

		Point2& operator+=(const Vector2<T>& vector) {
			m_x += vector.m_x;
			m_y += vector.m_y;

			return *this;
		}

		Point2& operator-=(const Vector2<T>& vector) {
			m_x -= vector.m_x;
			m_y -= vector.m_y;

			return *this;
		}


		T m_x;
		T m_y;
	private:
	};
}
#endif
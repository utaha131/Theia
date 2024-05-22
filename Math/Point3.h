#ifndef _THEIA_MATH_POINT3_H_
#define _THEIA_MATH_POINT3_H_
#include "../Types.h"
#include "Vector3.h"
#include <assert.h>
#include <algorithm>

namespace Theia {
	template<typename T> class Point3 {
	public:
		Point3() :
			m_x(T{}),
			m_y(T{}),
			m_z(T{})
		{

		}

		Point3(T x, T y, T z) :
			m_x(x),
			m_y(y),
			m_z(z)
		{

		}

		Point3(const Point3& point) :
			m_x(point.m_x),
			m_y(point.m_y),
			m_z(point.m_z)
		{

		}

		template <typename U> explicit Point3(const Point3<U>& point) :
			m_x(T(point.m_x)),
			m_y(T(point.m_y)),
			m_z(T(point.m_z))
		{

		}

		template <typename U> explicit Point3(U x, U y, U z) :
			m_x(T(x)),
			m_y(T(y)),
			m_z(T(z))
		{

		}

		Point3& operator=(const Point3& point) {
			m_x = point.m_x;
			m_y = point.m_y;
			m_z = point.m_z;

			return *this;
		}

		bool operator==(const Point3& point) const {
			return m_x == point.m_x && m_y == point.m_y && m_z == point.m_z;
		}

		bool operator!=(const Point3& point) const {
			return m_x != point.m_x || m_y != point.m_y || m_z != point.m_z;
		}

		T operator[](uint32_t index) const {
			assert(index < 3, "Point3 access index out of bounds.");
			if (index == 0) {
				return m_x;
			}
			else if (index == 1) {
				return m_y;
			}
			else {
				return m_z;
			}
		}

		T& operator[](uint32_t index) {
			assert(index < 3, "Point3 access index out of bounds.");
			if (index == 0) {
				return m_x;
			}
			else if (index == 1) {
				return m_y;
			}
			else {
				return m_z;
			}
		}

		Vector3<T> operator-(const Point3& point) const {
			return Vector3<T>(m_x - point.m_x, m_y - point.m_y, m_z - point.m_z);
		}

		Point3 operator+(const Vector3<T>& vector) const {
			return Point3(m_x + vector.m_x, m_y + vector.m_y, m_z + vector.m_z);
		}

		Point3 operator-(const Vector3<T>& vector) const {
			return Point3(m_x - vector.m_x, m_y - vector.m_y, m_z - vector.m_z);
		}

		template <typename U> Point3& operator+=(const Vector3<U>& vector) {
			m_x += vector.m_x;
			m_y += vector.m_y;
			m_z += vector.m_z;

			return *this;
		}

		template <typename U> Point3& operator-=(const Vector3<U>& vector) {
			m_x -= vector.m_x;
			m_y -= vector.m_y;
			m_z -= vector.m_z;

			return *this;
		}


		T m_x;
		T m_y;
		T m_z;
	private:
	};

	template <typename T> Point3<T> Min(const Point3<T>& point1, const Point3<T>& point2) {
		return Point3<T>(std::min(point1.m_x, point2.m_x), std::min(point1.m_y, point2.m_y), std::min(point1.m_z, point2.m_z));
	}

	template <typename T> Point3<T> Max(const Point3<T>& point1, const Point3<T>& point2) {
		return Point3<T>(std::max(point1.m_x, point2.m_x), std::max(point1.m_y, point2.m_y), std::max(point1.m_z, point2.m_z));
	}

	template <typename T> T DistanceSquared(const Point3<T>& point1, const Point3<T>& point2) {
		return (point1.m_x * point2.m_x) + (point1.m_y * point2.m_y) + (point1.m_z * point2.m_z);
	}

	template <typename T> auto Distance(const Point3<T>& point1, const Point3<T>& point2) {
		return std::sqrt(Theia::Float(DistanceSquared(point1, point2)));
	}
}
#endif
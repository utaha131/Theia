#ifndef _THEIA_MATH_VECTOR2_H_
#define _THEIA_MATH_VECTOR2_H_
#include "../Types.h"
#include <cmath>
#include <assert.h>
#include <array>

namespace Theia {
	template<typename T> class Vector2 {
	public:
		Vector2() :
			m_x(T{}),
			m_y(T{})
		{

		}

		Vector2(T value) :
			m_x(value),
			m_y(value)
		{

		}

		Vector2(T x, T y) :
			m_x(x),
			m_y(y)
		{

		}

		Vector2(const Vector2& vector) :
			m_x(vector.m_x),
			m_y(vector.m_y)
		{

		}

		template <typename U> explicit Vector2(const Vector2<U>& vector) :
			m_x(T(vector.m_x)),
			m_y(T(vector.m_y))
		{

		}

		/*template <typename U> explicit Vector2(U x, U y) :
			m_x(T(x)),
			m_y(T(y))
		{

		}*/

		Vector2& operator=(const Vector2& vector) {
			m_x = vector.m_x;
			m_y = vector.m_y;

			return *this;
		}

		bool operator==(const Vector2& vector) const {
			return m_x == vector.m_x && m_y == vector.m_y;
		}

		bool operator!=(const Vector2& vector) const {
			return m_x != vector.m_x || m_y != vector.m_y;
		}

		T operator[](uint32_t index) const {
			assert(index < 2, "Vector2 access index out of bounds.");
			if (index == 0) {
				return m_x;
			}
			else {
				return m_y;
			}
		}

		T& operator[](uint32_t index) {
			assert(index < 2, "Vector2 access index out of bounds.");
			if (index == 0) {
				return m_x;
			}
			else {
				return m_y;
			}
		}

		Vector2 operator-() const {
			return Vector2(-m_x, -m_y);
		}

		Vector2 operator+(const Vector2& vector) const {
			return Vector2(m_x + vector.m_x, m_y + vector.m_y);
		}

		Vector2 operator-(const Vector2& vector) const {
			return Vector2(m_x - vector.m_x, m_y - vector.m_y);
		}

		Vector2 operator*(const Vector2& vector) const {
			return Vector2(m_x * vector.m_x, m_y * vector.m_y);
		}

		Vector2 operator/(const Vector2& vector) const {
			return Vector2(m_x / vector.m_x, m_y / vector.m_y);
		}

		template <typename U> Vector2 operator*(U scalar) const {
			return Vector2(m_x * scalar, m_y * scalar);
		}

		template <typename U> Vector2 operator/(U scalar) const {
			return Vector2(m_x / scalar, m_y / scalar);
		}

		Vector2& operator+=(const Vector2& vector) {
			m_x += vector.m_x;
			m_y += vector.m_y;

			return *this;
		}

		Vector2& operator-=(const Vector2& vector) {
			m_x -= vector.m_x;
			m_y -= vector.m_y;

			return *this;
		}

		Vector2& operator*=(const Vector2& vector) {
			m_x *= vector.m_x;
			m_y *= vector.m_y;

			return *this;
		}

		Vector2& operator/=(const Vector2& vector) {
			m_x /= vector.m_x;
			m_y /= vector.m_y;

			return *this;
		}
		
		Vector2& operator*=(T scalar) {
			m_x *= scalar;
			m_y *= scalar;

			return *this;
		}

		Vector2& operator/=(T scalar) {
			m_x /= scalar;
			m_y /= scalar;

			return *this;
		}

		T m_x;
		T m_y;
	protected:
	private:
	};

	template<typename T, typename U> Vector2<T> operator*(U scalar, const Vector2<T>& vector) {
		return Vector2<T>(vector.m_x * scalar, vector.m_y * scalar);
	}

	template<typename T, typename U> Vector2<T> operator/(U scalar, const Vector2<T>& vector) {
		return Vector2<T>(scalar / vector.m_x, scalar / vector.m_y);
	}

	template<typename T> T LengthSquared(const Theia::Vector2<T>& vector) {
		return (vector.m_x * vector.m_x) + (vector.m_y * vector.m_y);
	}

	template<typename T> T Length(const Theia::Vector2<T>& vector) {
		return std::sqrt(LengthSquared(vector));
	}


	template<typename T> T Normalize(const Theia::Vector2<T>& vector) {
		return vector / Length(vector);
	}

	template<typename T> T Dot(const Theia::Vector2<T>& vector1, const Theia::Vector2<T>& vector2) {
		return (vector1.m_x * vector2.m_x) + (vector1.m_y * vector2.m_y);
	}
	
	template <typename T> Vector2<T> Abs(const Theia::Vector2<T>& vector) {
		return Vector2<T>(std::abs(vector.m_x), std::abs(vector.m_y));
	}

	template <typename T> Vector2<T> Ceil(const Theia::Vector2<T>& vector) {
		return Vector2<T>(std::ceil(vector.m_x), std::ceil(vector.m_y));
	}

	template <typename T> Vector2<T> Floor(const Theia::Vector2<T>& vector) {
		return Vector2<T>(std::floor(vector.m_x), std::floor(vector.m_y));
	}

	template <typename T> Vector2<T> Max(const Theia::Vector2<T>& vector1, const Theia::Vector2<T>& vector2) {
		return Vector2<T>(std::max(vector1.m_x, vector2.m_x), std::max(vector1.m_y, vector2.m_y));
	}

	template <typename T> Vector2<T> Min(const Theia::Vector2<T>& vector1, const Theia::Vector2<T>& vector2) {
		return Vector2<T>(std::min(vector1.m_x, vector2.m_x), std::min(vector1.m_y, vector2.m_y));
	}

	template <typename T> Vector2<T> Permute(const Theia::Vector2<T>& vector, const std::array<int, 2>& indices) {
		return Vector2<T>(vector[indices[0]], vector[indices[1]]);
	}

	template<typename T> inline Theia::Float MaxComponentValue(const Vector2<T>& vector) {
		return std::max({ vector.m_x, vector.m_y });
	}

	template<typename T> inline Theia::Float MinComponentValue(const Vector2<T>& vector) {
		return std::min({ vector.m_x, vector.m_y });
	}

	template<typename T> inline Theia::UInt32 MaxComponentIndex(const Vector2<T>& vector) {
		return (vector.m_x > vector.m_y) ? 0 : 1;
	}

	template<typename T> inline Theia::UInt32 MinComponentIndex(const Vector2<T>& vector) {
		return (vector.m_x < vector.m_y) ? 0 : 1;
	}
}
#endif
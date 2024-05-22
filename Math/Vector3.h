#ifndef _THEIA_MATH_VECTOR3_H_
#define _THEIA_MATH_VECTOR3_H_
#include "../Types.h"
#include <cmath>
#include <assert.h>

namespace Theia {
	template<typename T> class Vector3 {
	public:
		Vector3() :
			m_x(T{}),
			m_y(T{}),
			m_z(T{})
		{

		}

		Vector3(T value) :
			m_x(value),
			m_y(value),
			m_z(value)
		{

		}

		Vector3(T x, T y, T z) :
			m_x(x),
			m_y(y),
			m_z(z)
		{

		}

		Vector3(const Vector3& vector) :
			m_x(vector.m_x),
			m_y(vector.m_y),
			m_z(vector.m_z)
		{

		}

		template <typename U> explicit Vector3(U x, U y, U z) :
			m_x(T(x)),
			m_y(T(y)),
			m_z(T(z))
		{

		}

		template <typename U> explicit Vector3(const Vector3<U>& vector) :
			m_x(T(vector.m_x)),
			m_y(T(vector.m_y)),
			m_z(T(vector.m_z))
		{

		}

		Vector3& operator=(const Vector3& vector) {
			m_x = vector.m_x;
			m_y = vector.m_y;
			m_z = vector.m_z;
			
			return *this;
		}

		bool operator==(const Vector3& vector) const {
			return m_x == vector.m_x && m_y == vector.m_y && m_z == vector.m_z;
		}

		bool operator!=(const Vector3& vector) const {
			return m_x != vector.m_x || m_y != vector.m_y || m_z != vector.m_z;
		}

		T operator[](uint32_t index) const {
			assert(index < 3, "Vector3 access index out of bounds.");
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
			assert(index < 3, "Vector3 access index out of bounds.");
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

		Vector3 operator-() const {
			return Vector3(-m_x, -m_y, -m_z);
		}

		Vector3 operator+(const Vector3<T>& vector) const {
			return Vector3(m_x + vector.m_x, m_y + vector.m_y, m_z + vector.m_z);
		}
		
		Vector3 operator-(const Vector3<T>& vector) const {
			return Vector3(m_x - vector.m_x, m_y - vector.m_y, m_z - vector.m_z);
		}

		Vector3 operator*(const Vector3<T>& vector) const {
			return Vector3(m_x * vector.m_x, m_y * vector.m_y, m_z * vector.m_z);
		}

		Vector3 operator/(const Vector3<T>& vector) const {
			return Vector3(m_x / vector.m_x, m_y / vector.m_y, m_z / vector.m_z);
		}

		template <typename U> Vector3 operator*(U scalar) const {
			return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);
		}

		template <typename U> Vector3 operator/(U scalar) const {
			return Vector3(m_x / scalar, m_y / scalar, m_z / scalar);
		}

		Vector3& operator+=(const Vector3<T>& vector) {
			m_x += vector.m_x;
			m_y += vector.m_y;
			m_z += vector.m_z;
			return *this;
		}
		
		Vector3& operator-=(const Vector3<T>& vector) {
			m_x -= vector.m_x;
			m_y -= vector.m_y;
			m_z -= vector.m_z;
			return *this;
		}

		Vector3& operator*=(const Vector3<T>& vector) {
			m_x *= vector.m_x;
			m_y *= vector.m_y;
			m_z *= vector.m_z;
			return *this;
		}

		Vector3& operator/=(const Vector3<T>& vector) {
			m_x /= vector.m_x;
			m_y /= vector.m_y;
			m_z /= vector.m_z;
			return *this;
		}

		template <typename U> Vector3& operator*=(U scalar) {
			m_x *= scalar;
			m_y *= scalar;
			m_z *= scalar;

			return *this;
		}

		template <typename U> Vector3& operator/=(U scalar) {
			m_x /= scalar;
			m_y /= scalar;
			m_z /= scalar;

			return *this;
		}

		T m_x;
		T m_y;
		T m_z;
	protected:
	private:
	};

	template<typename T, typename U> Vector3<T> operator*(U scalar, const Vector3<T>& vector) {
		return Vector3<T>(vector.m_x * scalar, vector.m_y * scalar, vector.m_z * scalar);
	}

	template<typename T, typename U> Vector3<T> operator/(U scalar, const Vector3<T>& vector) {
		return Vector3<T>(scalar / vector.m_x, scalar / vector.m_y, scalar / vector.m_z);
	}

	template<typename T> T LengthSquared(const Theia::Vector3<T>& vector) {
		return (vector.m_x * vector.m_x) + (vector.m_y * vector.m_y) + (vector.m_z * vector.m_z);
	}

	template<typename T> T Length(const Theia::Vector3<T>& vector) {
		return std::sqrt(LengthSquared(vector));
	}

	template<typename T> Vector3<T>  Normalize(const Theia::Vector3<T>& vector) {
		return vector / Length(vector);
	}

	template<typename T> T Dot(const Theia::Vector3<T>& vector1, const Theia::Vector3<T>& vector2) {
		return (vector1.m_x * vector2.m_x) + (vector1.m_y * vector2.m_y) + (vector1.m_z * vector2.m_z);
	}

	template<typename T> Vector3<T>  Cross(const Theia::Vector3<T>& vector1, const Theia::Vector3<T>& vector2) {
		return Vector3<T>((vector1.m_y * vector2.m_z) - (vector1.m_z * vector2.m_y), (vector1.m_z * vector2.m_x) - (vector1.m_x * vector2.m_z), (vector1.m_x * vector2.m_y) - (vector1.m_y * vector2.m_x));
	}

	template <typename T> Vector3<T> Abs(const Theia::Vector3<T>& vector) {
		return Vector3<T>(std::abs(vector.m_x), std::abs(vector.m_y), std::abs(vector.m_z));
	}

	template <typename T> Vector3<T> Ceil(const Theia::Vector3<T>& vector) {
		return Vector3<T>(std::ceil(vector.m_x), std::ceil(vector.m_y), std::ceil(vector.m_z));
	}

	template <typename T> Vector3<T> Floor(const Theia::Vector3<T>& vector) {
		return Vector3<T>(std::floor(vector.m_x), std::floor(vector.m_y), std::floor(vector.m_z));
	}

	template <typename T> Vector3<T> Max(const Theia::Vector3<T>& vector1, const Theia::Vector3<T>& vector2) {
		return Vector3<T>(std::max(vector1.m_x, vector2.m_x), std::max(vector1.m_y, vector2.m_y), std::max(vector1.m_z, vector2.m_z));
	}

	template <typename T> Vector3<T> Min(const Theia::Vector3<T>& vector1, const Theia::Vector3<T>& vector2) {
		return Vector3<T>(std::min(vector1.m_x, vector2.m_x), std::min(vector1.m_y, vector2.m_y), std::min(vector1.m_z, vector2.m_z));
	}

	template <typename T> Vector3<T> Permute(const Theia::Vector3<T>& vector, const std::array<int, 3>& indices) {
		return Vector3<T>(vector[indices[0]], vector[indices[1]], vector[indices[2]]);
	}

	template<typename T> inline Theia::Float MaxComponentValue(const Vector3<T>& vector) {
		return std::max({ vector.m_x, vector.m_y, vector.m_z });
	}

	template<typename T> inline Theia::Float MinComponentValue(const Vector3<T>& vector) {
		return std::min({ vector.m_x, vector.m_y, vector.m_z });
	}

	template<typename T> inline Theia::UInt32 MaxComponentIndex(const Vector3<T>& vector) {
		if (vector.m_x > vector.m_y) {
			if (vector.m_x > vector.m_z) {
				return 0;
			} else {
				return 2;
			}
		}
		else {
			if (vector.m_y > vector.m_z) {
				return 1;
			}
			else {
				return 2;
			}
		}
	}

	template<typename T> inline Theia::UInt32 MinComponentIndex(const Vector3<T>& vector) {
		if (vector.m_x < vector.m_y) {
			if (vector.m_x < vector.m_z) {
				return 0;
			}
			else {
				return 2;
			}
		}
		else {
			if (vector.m_y < vector.m_z) {
				return 1;
			}
			else {
				return 2;
			}
		}
	}
}
#endif
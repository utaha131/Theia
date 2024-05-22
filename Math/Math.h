#ifndef _THEIA_MATH_H_
#define _THEIA_MATH_H_
#include "RandomNumberGenerator.h"
#include "Point2.h"
#include "Vector2.h"
#include "Point3.h"
#include "Vector3.h"
#include "Normal3.h"
#include "Ray.h"
#include "RayDifferential.h"
#include "AABB2.h"
#include "AABB3.h"
#include "SquareMatrix.h"
#include <string>
#include "Transform.h"
#include "Interval.h"

#include <iostream>


namespace Theia {
	using Point2i = Theia::Point2<Theia::Int32>;
	using Point2f = Theia::Point2<Theia::Float>;
	using Point3i = Theia::Point3<Theia::Int32>;
	using Point3f = Theia::Point3<Theia::Float>;
	using Point3Interval = Theia::Point3<Theia::Interval>;
	using Vector2i = Theia::Vector2<Theia::Int32>;
	using Vector2f = Theia::Vector2<Theia::Float>;
	using Vector3i = Theia::Vector3<Theia::Int32>;
	using Vector3f = Theia::Vector3<Theia::Float>;
	using Vector3Interval = Theia::Vector3<Theia::Interval>;
	using Normal3i = Theia::Normal3<Theia::Int32>;
	using Normal3f = Theia::Normal3<Theia::Float>;
	using AABB2i = Theia::AABB2<Theia::Int32>;
	using AABB2f = Theia::AABB2<Theia::Float>;
	using AABB3i = Theia::AABB3<Theia::Int32>;
	using AABB3f = Theia::AABB3<Theia::Float>;
	using Mat3 = Theia::SquareMatrix<Theia::Float, 3>;
	using Mat4 = Theia::SquareMatrix<Theia::Float, 4>;

	Mat4 CreateMat4(Theia::Float m00, Theia::Float m01, Theia::Float m02, Theia::Float m03,
					Theia::Float m10, Theia::Float m11, Theia::Float m12, Theia::Float m13,
					Theia::Float m20, Theia::Float m21, Theia::Float m22, Theia::Float m23,
					Theia::Float m30, Theia::Float m31, Theia::Float m32, Theia::Float m33
	);

	std::string Mat4ToSting(Theia::Mat4& matrix);
	Theia::Transform Translate(const Theia::Vector3f& delta);
	Theia::Transform Scale(const Theia::Vector3f& delta);

	Theia::Transform RotateXAxis(Theia::Float theta);
	Theia::Transform RotateYAxis(Theia::Float theta);
	Theia::Transform RotateZAxis(Theia::Float theta);
	Theia::Transform Rotate(Theia::Float sin_theta, Theia::Float cos_theta, const Vector3f& axis);
	Theia::Transform Rotate(Theia::Float theta, const Vector3f& axis);

	Theia::Transform LookAt(const Point3f& position, const Point3f& target, const Vector3f& up);
	Theia::Transform LookAt(const Point3f& position, const Vector3f& direction, const Vector3f& up);

	template<typename T, uint32_t N> inline std::array<T, N> operator*(const SquareMatrix<T, N>& matrix, const std::array<T, N>& arr) {
		std::array<T, N> return_array = std::array<T, N>();

		for (uint32_t i = 0; i < N; ++i) {
			return_array[i] = 0;
			for (uint32_t j = 0; j < N; ++j) {
				return_array[i] += matrix[i][j] * arr[j];
			}
		}
		
		return return_array;
	}

	template <typename T> inline Vector2<T> operator*(const SquareMatrix<T, 2>& matrix, const Vector2<T> vector) {
		Vector2<T> return_vector = Vector2<T>();

		for (uint32_t i = 0; i < 2; ++i) {
			for (uint32_t j = 0; j < 2; ++j) {
				return_vector[i] += matrix[i][j] * vector[j];
			}
		}

		return return_vector;
	}

	template <typename T> inline Vector3<T> operator*(const SquareMatrix<T, 3>& matrix, const Vector3<T> vector) {
		Vector3<T> return_vector = Vector3<T>();

		for (uint32_t i = 0; i < 3; ++i) {
			for (uint32_t j = 0; j < 3; ++j) {
				return_vector[i] += matrix[i][j] * vector[j];
			}
		}

		return return_vector;
	}

	template <typename T> bool IsPowerOf2(T value) {
		return value && !(value & (value - 1));
	}

	inline constexpr Int32 RoundUpPowerOf2(Int32 value) {
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		return value + 1;
	}

	inline constexpr Int64 RoundUpPowerOf2(Int64 value) {
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value |= value >> 32;
		return value + 1;
	}

	inline Float Lerp(Float x, Float a, Float b) {
		return (1.0f - x) * a + x * b;
	}
}
#endif
#ifndef _THEIA_MATH_TRANSFORM_H_
#define _THEIA_MATH_TRANSFORM_H_
#include "../Types.h"
#include "SquareMatrix.h"
#include "Point3.h"
#include "Vector3.h"
#include "Normal3.h"
#include "Ray.h"
#include "AABB3.h"
#include "Interval.h"

namespace Theia {
	class Transform {
	public:
		Transform() :
			m_matrix(Theia::SquareMatrix<Theia::Float32, 4>(1.0f)),
			m_inverse_matrix(Theia::SquareMatrix<Theia::Float32, 4>(1.0f))
		{

		}

		Transform(const SquareMatrix<Theia::Float32, 4>& matrix, const SquareMatrix<Theia::Float32, 4>& inverse_matrix) :
			m_matrix(matrix),
			m_inverse_matrix(inverse_matrix)
		{

		}


		Transform(const Transform& transform) :
			m_matrix(transform.m_matrix),
			m_inverse_matrix(transform.m_inverse_matrix)
		{

		}

		Transform& operator=(const Transform& transform) {
			m_matrix = transform.m_matrix;
			m_inverse_matrix = transform.m_inverse_matrix;
		}

		Transform operator*(const Transform& transform) const {
			return Transform(m_matrix * transform.m_matrix,  transform.m_inverse_matrix * m_inverse_matrix);
		}

		template<typename T> Theia::Point3<T> operator()(const Theia::Point3<T>& point) const {
			T x = m_matrix[0][0] * point.m_x + m_matrix[0][1] * point.m_y + m_matrix[0][2] * point.m_z + m_matrix[0][3];
			T y = m_matrix[1][0] * point.m_x + m_matrix[1][1] * point.m_y + m_matrix[1][2] * point.m_z + m_matrix[1][3];
			T z = m_matrix[2][0] * point.m_x + m_matrix[2][1] * point.m_y + m_matrix[2][2] * point.m_z + m_matrix[2][3];
			T w = m_matrix[3][0] * point.m_x + m_matrix[3][1] * point.m_y + m_matrix[3][2] * point.m_z + m_matrix[3][3];

			if (w == 1.0f) {
				return Theia::Point3<T>(x, y, z);
			}
			else {
				return Theia::Point3<T>(x / w, y / w, z / w);
			}
		}

		template <typename T> Theia::Point3<T> ApplyInverse(const Theia::Point3<T>& point) const {
			T x = m_inverse_matrix[0][0] * point.m_x + m_inverse_matrix[0][1] * point.m_y + m_inverse_matrix[0][2] * point.m_z + m_inverse_matrix[0][3];
			T y = m_inverse_matrix[1][0] * point.m_x + m_inverse_matrix[1][1] * point.m_y + m_inverse_matrix[1][2] * point.m_z + m_inverse_matrix[1][3];
			T z = m_inverse_matrix[2][0] * point.m_x + m_inverse_matrix[2][1] * point.m_y + m_inverse_matrix[2][2] * point.m_z + m_inverse_matrix[2][3];
			T w = m_inverse_matrix[3][0] * point.m_x + m_inverse_matrix[3][1] * point.m_y + m_inverse_matrix[3][2] * point.m_z + m_inverse_matrix[3][3];

			if (w == 1.0f) {
				return Theia::Point3<T>(x, y, z);
			}
			else {
				return Theia::Point3<T>(x, y, z) / w;
			}
		}

		template<typename T> Theia::Vector3<T> operator()(const Theia::Vector3<T>& vector) const {
			T x = m_matrix[0][0] * vector.m_x + m_matrix[0][1] * vector.m_y + m_matrix[0][2] * vector.m_z;
			T y = m_matrix[1][0] * vector.m_x + m_matrix[1][1] * vector.m_y + m_matrix[1][2] * vector.m_z;
			T z = m_matrix[2][0] * vector.m_x + m_matrix[2][1] * vector.m_y + m_matrix[2][2] * vector.m_z;

			return Theia::Vector3<T>(x, y, z);
		}

		template<typename T> Theia::Vector3<T> ApplyInverse(const Theia::Vector3<T>& vector) const {
			T x = m_inverse_matrix[0][0] * vector.m_x + m_inverse_matrix[0][1] * vector.m_y + m_inverse_matrix[0][2] * vector.m_z;
			T y = m_inverse_matrix[1][0] * vector.m_x + m_inverse_matrix[1][1] * vector.m_y + m_inverse_matrix[1][2] * vector.m_z;
			T z = m_inverse_matrix[2][0] * vector.m_x + m_inverse_matrix[2][1] * vector.m_y + m_inverse_matrix[2][2] * vector.m_z;

			return Theia::Vector3<T>(x, y, z);
		}

		template <typename T> Theia::Normal3<T> operator()(const Theia::Normal3<T>& normal) const {
			T x = m_inverse_matrix[0][0] * normal.m_x + m_inverse_matrix[1][0] * normal.m_y + m_inverse_matrix[2][0] * normal.m_z;
			T y = m_inverse_matrix[0][1] * normal.m_x + m_inverse_matrix[1][1] * normal.m_y + m_inverse_matrix[2][1] * normal.m_z;
			T z = m_inverse_matrix[0][2] * normal.m_x + m_inverse_matrix[1][2] * normal.m_y + m_inverse_matrix[2][2] * normal.m_z;

			return Theia::Normal3<T>(x, y, z);
		}

		Theia::Ray operator()(const Ray& ray, Theia::Float& time_max) const {
			Point3<Theia::Interval> origin = (*this)(Theia::Point3<Theia::Interval>(ray.GetOrigin()));
			Vector3<Theia::Float> direction = (*this)(ray.GetDirection());
			return Ray(Point3<Theia::Float>(origin), direction, ray.GetTime(), ray.GetMedium());
		}

		Theia::AABB3<Theia::Float> operator()(const Theia::AABB3<Theia::Float>& aabb) const {
			Theia::AABB3<Theia::Float> return_aabb;

			for (uint32_t i = 0; i < 8; ++i) {
				return_aabb = Theia::Union(return_aabb, (*this)(aabb.Corner(i)));
			}

			return return_aabb;
		}

		bool SwapsHandedness() const {
			//TODO implement Check of SwapHandedness.
			return false;
		}
	private:
		Theia::SquareMatrix<Theia::Float32, 4> m_matrix, m_inverse_matrix;
	};
}
#endif
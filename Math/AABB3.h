#ifndef _THEIA_MATH_AABB3_H_
#define _THEIA_MATH_AABB3_H_
#include "Point3.h"
#include "Vector3.h"
#include <limits>

namespace Theia {
	template<typename T> class AABB3 {
	public:
		AABB3() {
			T min = std::numeric_limits<T>::lowest();
			T max = std::numeric_limits<T>::max();
			m_min = Point3<T>(max, max, max);
			m_max = Point3<T>(min, min, min);
		}

		Point3<T> operator[](uint32_t index) const {
			return (index == 0) ? m_min : m_max;
		}

		Point3<T>& operator[](uint32_t index) {
			return (index == 0) ? m_min : m_max;
		}

		Point3<T> Corner(uint32_t index) const {
			return Point3<T>(
				(*this)[(index & 1)].m_x,
				(*this)[(index & 2) ? 1 : 0].m_y,
				(*this)[(index & 4) ? 1 : 0].m_z
			);
		}

		Vector3<T> Diagonal() {
			return m_max - m_min;
		}

		T Volume() {
			Vector3<T> diagonal = m_max - m_min;
			return diagonal.m_x * diagonal.m_y * diagonal.m_z;
		}

		Point3<T> m_min, m_max;
	private:
	};

	template <typename T> AABB3<T> Union(const AABB3<T>& aabb, const Point3<T>& point) {
		AABB3<T> return_aabb;

		return_aabb.m_min = Min(aabb.m_min, point);
		return_aabb.m_max = Max(aabb.m_max, point);

		return return_aabb;
	}

	template <typename T> AABB3<T> Union(const AABB3<T>& aabb1, const AABB3<T>& aabb2) {
		AABB3<T> return_aabb;

		return_aabb.m_min = Min(aabb1.m_min, aabb2.m_min);
		return_aabb.m_max = Max(aabb1.m_max, aabb2.m_max);

		return return_aabb;
	}

	template <typename T, typename U> T DistanceSquared(const Point3<T>& point, const AABB3<U>& aabb) {

	}

	template <typename T, typename U> T Distance(const Point3<T>& point, const AABB3<U>& aabb) {

	}
}
#endif
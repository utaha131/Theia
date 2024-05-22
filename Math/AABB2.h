#ifndef _THEIA_MATH_AABB2_H_
#define _THEIA_MATH_AABB2_H_
#include "Point2.h"
#include <limits>

namespace Theia {
	template<typename T> class AABB2 {
	public:
		AABB2() {
			T min = std::numeric_limits<T>::lowest();
			T max = std::numeric_limits<T>::max();
			m_min = Point2<T>(max);
			m_max = Point2<T>(min);
		}

		Vector2<T> Diagonal() { return m_max - m_min; }
		
		T Area() {
			Vector2<T> diagonal = m_max - m_min;
			return diagonal.m_x * diagonal.m_y;
		}


	private:
		Theia::Point2<T> m_min, m_max;
	};
}
#endif
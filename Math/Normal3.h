#ifndef _THEIA_MATH_NORMAL3_H_
#define _THEIA_MATH_NORMAL3_H_

namespace Theia {
	template<typename T> class Normal3 {
	public:
		Normal3() :
			m_x(T{}),
			m_y(T{}),
			m_z(T{})
		{

		}

		Normal3(T x, T y, T z) :
			m_x(x),
			m_y(y),
			m_z(z)
		{

		}

		Normal3(const Normal3& normal) :
			m_x(normal.m_x),
			m_y(normal.m_y),
			m_z(normal.m_z)
		{

		}

		template <typename U> explicit Normal3(const Normal3<U>& normal) :
			m_x(T(normal.m_x)),
			m_y(T(normal.m_y)),
			m_z(T(normal.m_z))
		{

		}

		Normal3& operator=(const Normal3& normal) {
			m_x = normal.m_x;
			m_y = normal.m_y;
			m_z = normal.m_z;

			return *this;
		}

		bool operator==(const Normal3& normal) const {
			return m_x == normal.m_x && m_y == normal.m_y && m_z == normal.m_z;
		}

		bool operator!=(const Normal3& normal) const {
			return m_x != normal.m_x || m_y != normal.m_y || m_z != normal.m_z;
		}

		Normal3 operator+(const Normal3& normal) const {
			return Normal3(m_x + normal.m_x, m_y + normal.m_y, m_z + normal.m_z);
		}

		Normal3 operator*(const Normal3& normal) const {

		}

		Normal3& operator+=(const Normal3& normal) {
			m_x += normal.m_x;
			m_y += normal.m_y;
			m_z += normal.m_z;

			return *this;
		}
		

		T m_x;
		T m_y;
		T m_z;
	private:
	};
}
#endif
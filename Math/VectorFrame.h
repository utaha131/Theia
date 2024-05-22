#ifndef _THEIA_MATH_VECTOR_FRAME_H_
#define _THEIA_MATH_VECTOR_FRAME_H_
#include "../Types.h"
#include "Vector3.h"

namespace Theia {
	class VectorFrame {
	public:
		VectorFrame() :
			m_x(Theia::Vector3<Theia::Float>(1.0, 0.0, 0.0)),
			m_y(Theia::Vector3<Theia::Float>(0.0, 1.0, 0.0)),
			m_z(Theia::Vector3<Theia::Float>(0.0, 0.0, 1.0))
		{

		}

		Theia::Vector3<Theia::Float> m_x, m_y, m_z;
	};
}
#endif
#ifndef _THEIA_MATH_RAY_DIFFERENTIAL_H_
#define _THEIA_MATH_RAY_DIFFERENTIAL_H_
#include "../Types.h"
#include "Point3.h"
#include "Vector3.h"
#include "Ray.h"
#include "IMedium.h"

namespace Theia {

	class RayDifferential {
	public:
		RayDifferential(const Theia::Point3<Theia::Float>& origin, const Theia::Vector3<Theia::Float>& direction, Theia::Float time = 0.0f, Medium medium = nullptr);
		RayDifferential(const Ray& ray);
		void ScaleDifferential(Theia::Float s);
	private:
		Theia::Point3<Theia::Float> m_origin, m_rx_origin, m_ry_origin;
		Theia::Vector3<Theia::Float> m_direction, m_rx_direction, m_ry_direction;
		Theia::Float m_time;
		Theia::Medium m_medium;
	};
}
#endif
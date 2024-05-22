#ifndef _THEIA_MATH_RAY_H_
#define _THEIA_MATH_RAY_H_
#include "Point3.h"
#include "Vector3.h"
#include "IMedium.h"

namespace Theia {
	class Ray {
	public:
		Ray() = delete;
		Ray(const Theia::Point3<Theia::Float>& origin, const Theia::Vector3<Theia::Float>& direction, Theia::Float time = 0.0f, Medium medium = nullptr);
		Theia::Point3<Float> operator()(float t) const;
		Theia::Point3<Theia::Float> GetOrigin() const;
		Theia::Vector3<Theia::Float> GetDirection() const;
		Theia::Float GetTime() const;
		Theia::Medium GetMedium() const;
	protected:
	private:
		Theia::Point3<Theia::Float> m_origin;
		Theia::Vector3<Theia::Float> m_direction;
		Theia::Float m_time;
		Theia::Medium m_medium = nullptr;
	};
}
#endif
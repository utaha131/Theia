#include "Ray.h"

namespace Theia {
	Ray::Ray(const Theia::Point3<Theia::Float>& origin, const Theia::Vector3<Theia::Float>& direction, Theia::Float time, Medium medium) :
		m_origin(origin),
		m_direction(direction),
		m_time(time),
		m_medium(medium)
	{

	}

	Theia::Point3<Float> Ray::operator()(float t) const {
		return m_origin + (t * m_direction); 
	}

	Theia::Point3<Theia::Float> Ray::GetOrigin() const {
		return m_origin; 
	}

	Theia::Vector3<Theia::Float> Ray::GetDirection() const {
		return m_direction; 
	}

	Theia::Float Ray::GetTime() const {
		return m_time;
	}

	Theia::Medium Ray::GetMedium() const {
		return m_medium;
	}
}
#include "RayDifferential.h"

namespace Theia {

	RayDifferential::RayDifferential(const Theia::Point3<Theia::Float>& origin, const Theia::Vector3<Theia::Float>& direction, Theia::Float time, Medium medium) :
		m_origin(origin),
		m_direction(direction),
		m_time(time),
		m_medium(medium)
	{

	}

	RayDifferential::RayDifferential(const Ray& ray) :
		m_origin(ray.GetOrigin()),
		m_direction(ray.GetDirection())

	{

	}

	void RayDifferential::ScaleDifferential(Theia::Float s) {
		m_rx_origin = m_origin + (m_rx_origin - m_origin) * s;
		m_ry_origin = m_origin + (m_ry_origin - m_origin) * s;
		m_rx_direction = m_direction + (m_rx_direction - m_direction) * s;
		m_ry_direction = m_direction + (m_ry_direction - m_direction) * s;
	}
}
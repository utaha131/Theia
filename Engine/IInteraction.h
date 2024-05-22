#ifndef _THEIA_ENGINE_I_INTERACTION_H_
#define _THEIA_ENGINE_I_INTERACTION_H_
#include "../Math/Math.h"
namespace Theia {
	class IInteraction {
	public:
		IInteraction() :
			m_point_interval({}),
			m_time(0.0),
			m_w_o({}),
			m_normal({}),
			m_uv({}),
			m_medium_pair(nullptr)

		{

		}

		IInteraction(const Theia::Point3Interval& point_interval, Theia::Float time, const Theia::Vector3f& w_o, const Theia::Normal3f& normal, const Theia::Point2f& uv, const Theia::Medium medium) :
			m_point_interval(point_interval),
			m_time(time),
			m_w_o(w_o),
			m_normal(normal),
			m_uv(uv),
			m_medium_pair(medium)
		{

		}

		IInteraction(const Theia::Point3Interval& point_interval, Theia::Float time, const Theia::Vector3f& w_o, const Theia::Normal3f& normal, const Theia::Point2f& uv, const Theia::MediumPair& medium_pair) :
			m_point_interval(point_interval),
			m_time(time),
			m_w_o(w_o),
			m_normal(normal),
			m_uv(uv),
			m_medium_pair(medium_pair)
		{

		}
		
		Point3Interval m_point_interval;
		Theia::Float m_time;
		Theia::Vector3f m_w_o;
		Theia::Normal3f m_normal;
		Theia::Point2f m_uv;
		Theia::MediumPair m_medium_pair;
	};

	typedef IInteraction* Interaction;
}
#endif
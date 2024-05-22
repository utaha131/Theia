#ifndef _THEIA_MATH_I_MEDIUM_H_
#define _THEIA_MATH_I_MEDIUM_H_

namespace Theia {
	class IMedium {

	};

	typedef IMedium* Medium;

	class MediumPair {
	public:
		MediumPair() :
			m_inside(nullptr),
			m_outside(nullptr)
		{

		}

		MediumPair(const Medium medium) :
			m_inside(medium),
			m_outside(medium)
		{

		}

		MediumPair(const Medium inside, const Medium outside) :
			m_inside(inside),
			m_outside(outside)
		{

		}

		bool IsMediumTransition() const {
			return m_inside != m_outside;
		}

		Medium m_inside;
		Medium m_outside;
	};
}
#endif
#ifndef _THEIA_RADIOMETRY_CONSTANT_SPECTRUM_H_
#define _THEIA_RADIOMETRY_CONstANT_SPECTRUM_H_
#include "ISpectrum.h"

namespace Theia {
	class ConstantSpectrum : public ISpectrum {
	public:
		ConstantSpectrum(Theia::Float constant) :
			m_constant(constant)
		{}
		Theia::Float operator()(Theia::Float lambda) const override { return m_constant; }
		Theia::Float MaxValue() const override { return m_constant; }
	private:
		Theia::Float m_constant;
	};
}
#endif
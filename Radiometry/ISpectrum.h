#ifndef _THEIA_RADIOMETRY_I_SPECTRUM_H_
#define _THEIA_RADIOMETRY_I_SPECTRUM_H_
#include <string>
#include "../Types.h"

namespace Theia {

	constexpr Float Lambda_min = 360, Lambda_max = 830;

	class ISpectrum {
	public:
		virtual Theia::Float operator()(Theia::Float lambda) const = 0;
		virtual Theia::Float MaxValue() const = 0;
	private:
	};

	typedef ISpectrum* Spectrum;
}
#endif
#ifndef _THEIA_RADIOMETRY_DENSELY_SAMPLED_SPECTRUM_H_
#define _THEIA_RADIOMETRY_DENSELY_SAMPLED_SPECTRUM_H_
#include "ISpectrum.h"
#include "../Types.h"
#include <vector>


namespace Theia {
	class DenselySampledSpectrum : ISpectrum {
	public:
		DenselySampledSpectrum(UInt32 lambda_min, UInt32 lambda_max)  :
			m_lambda_min(lambda_min),
			m_lambda_max(lambda_max),
			m_values(std::vector<Float>(m_lambda_max - m_lambda_min + 1))
		{
		
		}

		DenselySampledSpectrum(const Spectrum spectrum) :
			m_lambda_min(Lambda_min),
			m_lambda_max(Lambda_max),
			m_values(std::vector<Float>(m_lambda_max - m_lambda_min + 1, {}))
		{
			if (spectrum != nullptr) {
				for (uint32_t lambda = m_lambda_min; lambda <= m_lambda_max; ++lambda) {
					m_values[lambda - m_lambda_min] = (*spectrum)(lambda);
				}
			}
		}

		Theia::Float operator()(Theia::Float lambda) const override {
			UInt32 offset = std::lround(lambda) - m_lambda_min;
			if (offset < 0 || offset >= m_values.size()) {
				return 0;
			}

			return m_values[offset];
		}

		Theia::Float MaxValue() const override {
			return *std::max_element(m_values.begin(), m_values.end());
		}
	private:
		Theia::UInt32 m_lambda_min, m_lambda_max;
		std::vector<Float> m_values;
	};
}
#endif
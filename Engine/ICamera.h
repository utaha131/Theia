#ifndef _THEIA_I_CAMERA_H_
#define _THEIA_I_CAMERA_H_
#include "../Math/Math.h"
#include <optional>

namespace Theia {
	typedef struct CameraSample {
		Point2f Film_Point;
		Point2f Point_Lens;
		Float Time = 0.0;
		Float Filter_Weight = 1.0;
	} CameraSample;

	typedef struct CameraRay {
		Ray Ray;
		//SampledSpectrum weight;
	} CameraRay;

	typedef struct CameraRayDifferential {
		RayDifferential Ray_Differential;
		//SampledSpectrum weight;
	} CameraRayDifferential;

	class ICamera {
	public:
		virtual void GenerateRay() const = 0;
		virtual std::optional<CameraRayDifferential> GenerateRayDifferentials(const CameraSample& sample/*, SampledWaveLength& lambda*/) const = 0;
	protected:
	private:
	};
}
#endif
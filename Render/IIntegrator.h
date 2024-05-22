#ifndef _THEIA_I_INTEGRATOR_H_
#define _THEIA_I_INTEGRATOR_H_

namespace Theia {
	class IIntegrator {
	public:
		virtual ~IIntegrator();
		virtual void Integrate() = 0;
	protected:
	private:
	};
}
#endif
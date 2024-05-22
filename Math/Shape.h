#ifndef _THEIA_MATH_SHAPE_H_
#define _THEIA_MATH_SHAPE_H_
#include "../Types.h"

namespace Theia {

	struct ShapeIntersection {

		Theia::Float32 time;
	};

	class Shape {
	public:
		bool Intersect() const = 0;
	private:
	};
}
#endif
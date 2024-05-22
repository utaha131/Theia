//#include "Math/Math.h"
//#include "Engine/Engine.h"
//#include <iostream>
//
//int main(int argc, char* argv[]) {
//	Theia::Point2f p;
//	Theia::Vector2f f;
//	Theia::Vector2i i;
//	p + f * 2.0f;
//	Theia::Mat4 m = Theia::CreateMat4(1, 2, 3, 4,
//										4, 3, 2, 1,
//										1, 2, 3, 4,
//										4, 3, 2, 1);
//	Theia::Mat4 m2 = Theia::CreateMat4(5, 6, 7, 8,
//										8, 7, 6, 5,
//										5, 6, 7, 8,
//										8, 7, 6, 5);
//
//	m *= m2;
//	m = (2.0f * m) - 6.0f;
//
//	std::cout << Theia::Mat4ToSting(m) << std::endl;
//	std::cout << (f == Theia::Vector2f(i)) << std::endl;
//
//	return 0;
//}
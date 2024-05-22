#ifndef _THEIA_TYPES_H_
#define _THEIA_TYPES_H_
#include <stdint.h>
#include <bit>
#include <cmath>

namespace Theia {
	using Float32 = float;
	using Float64 = double;
	using FloatBits32 = uint32_t;
	using FloatBits64 = uint64_t;
	using UInt32 = uint32_t;
	using UInt64 = uint64_t;
	using Int32 = int32_t;
	using Int64 = int64_t;

	using Float = Theia::Float32;
	using FloatBits = Theia::FloatBits32;

	static_assert(sizeof(Float32) == sizeof(uint32_t), "Float32 size is not 32-bits");
	static_assert(sizeof(Float64) == sizeof(uint64_t), "Float64 size is not 64-bits");


	inline bool IsInfinity(Theia::Float value) {
		return std::isinf(value);
	}

	inline Theia::FloatBits FloatToFloatBits(Theia::Float value) {
		return std::bit_cast<Theia::FloatBits>(value);
	}

	inline Theia::Float FloatBitsToFloat(Theia::FloatBits value) {
		return std::bit_cast<Theia::Float>(value);
	}

	inline Theia::Float NextFloatUp(Theia::Float value) {
		if (Theia::IsInfinity(value) && value > 0.0f) {
			return value;
		}

		if (value == -0.0f) {
			value = 0.0f;
		}
		FloatBits bits = FloatToFloatBits(value);

		if (value >= 0) {
			++bits;
		}
		else {
			--bits;
		}

		return FloatBitsToFloat(bits);
	}

	inline Theia::Float NextFloatDown(Theia::Float value) {
		if (Theia::IsInfinity(value) && value < 0.0f) {
			return value;
		}

		if (value == 0.0f) {
			value = -0.0f;
		}
		FloatBits bits = FloatToFloatBits(value);

		if (value > 0) {
			--bits;
		}
		else {
			++bits;
		}

		return FloatBitsToFloat(bits);
	}

	constexpr Theia::Float Infinity = std::numeric_limits<Theia::Float>::infinity();
}
#endif
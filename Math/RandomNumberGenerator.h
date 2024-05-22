#ifndef _THEIA_MATH_RANDOM_NUMBER_GENERATOR_H_
#define _THEIA_MATH_RANDOM_NUMBER_GENERATOR_H_
#include "../Types.h"
#include "../ext/pcg/pcg_basic.h"
#include <type_traits>
#include <algorithm>

#define PCG32_DEFAULT_STATE 0x853c49e6748fea9bULL
#define PCG32_DEFAULT_STREAM 0xda3e39cb94b95bdbULL
#define PCG32_MULT 0x5851f42d4c957f2dULL

namespace Theia {
	inline uint64_t MixBits(uint64_t v) {
		v ^= (v >> 31);
		v *= 0x7fb5d329728ea185;
		v ^= (v >> 27);
		v *= 0x81dadef4bc2dd44d;
		v ^= (v >> 33);
		return v;
	}

	class RandomNumberGenerator {
	public:		
		RandomNumberGenerator() :
			m_state(PCG32_DEFAULT_STATE),
			m_inc(PCG32_DEFAULT_STREAM)
		{
		
		}

		RandomNumberGenerator(UInt64 sequence_index) {
			SetSequence(sequence_index, MixBits(sequence_index));
		}

		RandomNumberGenerator(UInt64 sequence_index, UInt64 offset) {
			SetSequence(sequence_index, offset);
		}

		template <typename T> T Uniform();

		template <typename T> typename std::enable_if_t<std::is_integral_v<T>, T> Uniform(T value) {
			T threshold = (~value + 1u) % value;
			while (true) {
				T v = Uniform<T>();
				if (v >= threshold) {
					return v % value;
				}
			}
		}
		
		void Advance(UInt64 delta) {
			UInt64 mult = PCG32_MULT, inc = m_inc;
			UInt64 acc_mult = 1u, acc_inc = 0u;

			while (delta > 0) {
				if (delta & 1) {
					acc_mult *= mult;
					acc_inc = acc_inc * mult + inc;
				}
				inc = (mult + 1) * inc;
				mult *= mult;
				delta /= 2;
			}

			m_state = m_state * acc_mult + acc_inc;
		}

		void SetSequence(UInt64 sequence_index, UInt64 seed);

		void SetSequence(UInt64 sequence_index) {
			SetSequence(sequence_index, MixBits(sequence_index));
		}

		Int64 operator-(const RandomNumberGenerator& rng) const {
			uint64_t curMult = PCG32_MULT, curPlus = m_inc, curState = rng.m_state;
			uint64_t theBit = 1u, distance = 0u;
			while (m_state != curState) {
				if ((m_state & theBit) != (curState & theBit)) {
					curState = curState * curMult + curPlus;
					distance |= theBit;
				}
				//CHECK_EQ(state & theBit, curState & theBit);
				theBit <<= 1;
				curPlus = (curMult + 1ULL) * curPlus;
				curMult *= curMult;
			}
			return (int64_t)distance;
		}
	private:
		UInt64 m_state, m_inc;
	};

	template <typename T>
	inline T RandomNumberGenerator::Uniform() {
		return T::unimplemented;
	};

	template <> inline UInt32 RandomNumberGenerator::Uniform<UInt32>() {
		UInt64 old_state = m_state;
		m_state = old_state * PCG32_MULT + m_inc;
		UInt32 xor_shifted = (UInt32)(((old_state >> 18u) ^ old_state) >> 27u);
		UInt32 rotation32 = (UInt32)(old_state >> 59u);
		return (xor_shifted >> rotation32) | (xor_shifted << ((~rotation32 + 1u) & 31));
	}

	template <> inline UInt64 RandomNumberGenerator::Uniform<UInt64>() {
		UInt64 v0 = Uniform<UInt32>(), v1 = Uniform<UInt32>();
		return (v0 << 32) | v1;
	}

	template <> inline Int32 RandomNumberGenerator::Uniform<Int32>() {
		UInt32 v = Uniform<UInt32>();
		if (v <= (UInt32)std::numeric_limits<Int32>::max()) {
			return Int32(v);
		}
		if (v >= (UInt32)std::numeric_limits<Int32>::min()) {
			return Int32(v - std::numeric_limits<Int32>::min()) + std::numeric_limits<Int32>::min();
		}
	}

	template <> inline Int64 RandomNumberGenerator::Uniform<Int64>() {
		UInt64 v = Uniform<UInt64>();
		if (v <= (UInt64)std::numeric_limits<Int64>::max()) {
			return Int64(v);
		}
		if (v >= (UInt64)std::numeric_limits<Int64>::min()) {
			return Int64(v - std::numeric_limits<Int64>::min()) + std::numeric_limits<Int64>::min();
		}
	}

	template <> inline Float32 RandomNumberGenerator::Uniform<Float32>() {
		Float32 ep = 0x1.fffffep-1;
		return std::min<Float32>(ep, Uniform<UInt32>() * 0x1p-32f);
	}

	template <> inline Float64 RandomNumberGenerator::Uniform<Float64>() {
		return std::min<Float64>(0x1.fffffffffffffp-1, Uniform<UInt64>() * 0x1p-64);
	}

	inline void RandomNumberGenerator::SetSequence(UInt64 sequence_index, UInt64 seed) {
		m_state = 0u;
		m_inc = (sequence_index << 1u) | 1u;
		Uniform<UInt32>();
		m_state += seed;
		Uniform<UInt32>();
	}
}
#endif
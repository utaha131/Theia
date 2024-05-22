#ifndef _THEIA_MATH_INTERVAL_H_
#define _THEIA_MATH_INTERVAL_H_
#include "../Types.h"

namespace Theia {
	class Interval {
	public:
		Interval();
		explicit Interval(Theia::Float value);
		Interval(Theia::Float low, Theia::Float high);

		static Interval FromValueAndError(Float value, Float err) {
			Interval i;
			if (err == 0)
				i.m_low = i.m_high = value;
			else {
				i.m_low = NextFloatDown(value - err);
				i.m_high = NextFloatUp(value + err);
			}
			return i;
		}

		explicit operator Theia::Float() const;
		bool operator==(const Interval& interval) const;
		bool operator==(Theia::Float value) const;
		Interval& operator=(Theia::Float value);
		Interval operator-() const;
		bool operator!=(Theia::Float value) const;
		Interval operator+(const Interval& interval) const;
		Interval operator-(const Interval& interval) const;
		Interval operator*(const Interval& interval) const;
		Interval operator/(const Interval& interval) const;
		Interval operator+(Theia::Float value) const;
		Interval operator-(Theia::Float value) const;
		Interval operator*(Theia::Float value) const;
		Interval operator/(Theia::Float value) const;

		Interval& operator+=(const Interval& interval) {
			m_low = NextFloatDown(m_low + interval.m_low);
			m_high = NextFloatUp(m_high + interval.m_high);

			return *this;
		}
		/*Interval& operator-=(const Interval& interval);
		Interval& operator*=(const Interval& interval);
		Interval& operator/=(const Interval& interval);*/

		Interval& operator+=(Theia::Float value);
		/*Interval operator-=(Theia::Float value) const;
		Interval operator*=(Theia::Float value) const;
		Interval operator/=(Theia::Float value) const;*/

		Theia::Float GetHigh() const;
		Theia::Float GetLow() const;
		Theia::Float GetMidpoint() const;
		Theia::Float GetRange() const;
	private:
		Theia::Float m_low, m_high;
	};

	Interval operator+(Theia::Float value, const Interval& interval);
	Interval operator-(Theia::Float value, const Interval& interval);
	Interval operator*(Theia::Float value, const Interval& interval);
	Interval operator/(Theia::Float value, const Interval& interval);
	bool InRange(Theia::Float value, const Interval& interval);
	bool InRange(const Interval& interval1, const Interval& interval2);
}
#endif
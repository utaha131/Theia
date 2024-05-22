#include "Interval.h"

namespace Theia {
	Interval::Interval() :
		m_low(0.0),
		m_high(0.0)
	{

	}

	Interval::Interval(Theia::Float value) :
		m_low(value),
		m_high(value)
	{

	}

	Interval::Interval(Theia::Float low, Theia::Float high) :
		m_low(std::min(low, high)),
		m_high(std::max(low, high))
	{

	}

	Interval::operator Theia::Float() const {
		return GetMidpoint();
	}

	bool Interval::operator==(const Interval& interval) const {
		return m_low == interval.m_low && m_high == interval.m_high;
	}

	bool Interval::operator==(Theia::Float value) const {
		return m_low == value && m_high == value;
	}

	Interval& Interval::operator=(Theia::Float value) {
		m_low = m_high = value;
		return *this;
	}

	Interval Interval::operator-() const {
		return Interval(-m_low, -m_high);
	}

	bool Interval::operator!=(Theia::Float value) const {
		return value < m_low || value > m_high;
	}

	Interval Interval::operator+(const Interval& interval) const {
		return Interval(NextFloatDown(m_low + interval.m_low), NextFloatUp(m_high + interval.m_high));
	}

	Interval Interval::operator-(const Interval& interval) const {
		return Interval(NextFloatDown(m_low - interval.m_high), NextFloatUp(m_high - interval.m_low));
	}

	Interval Interval::operator*(const Interval& interval) const {
		return Interval(
			std::min({ NextFloatDown(m_low * interval.m_low), NextFloatDown(m_low * interval.m_high), NextFloatDown(m_high * interval.m_low), NextFloatDown(m_high * interval.m_high) }),
			std::max({ NextFloatUp(m_low * interval.m_low), NextFloatUp(m_low * interval.m_high), NextFloatUp(m_high * interval.m_low), NextFloatUp(m_high * interval.m_high) })
		);
	}

	Interval Interval::operator/(const Interval& interval) const {
		if (InRange(0, interval)) {
			return Interval(-Theia::Infinity, Theia::Infinity);
		}

		return Interval(
			std::min({ NextFloatDown(m_low / interval.m_low), NextFloatDown(m_low / interval.m_high), NextFloatDown(m_high / interval.m_low), NextFloatDown(m_high / interval.m_high) }),
			std::max({ NextFloatUp(m_low / interval.m_low), NextFloatUp(m_low / interval.m_high), NextFloatUp(m_high / interval.m_low), NextFloatUp(m_high / interval.m_high) })
		);
	}

	Interval Interval::operator+(Theia::Float value) const {
		return (*this) + Interval(value);
	}

	Interval Interval::operator-(Theia::Float value) const {
		return (*this) - Interval(value);
	}

	Interval Interval::operator*(Theia::Float value) const {
		if (value > 0.0) {
			return Interval(NextFloatDown(value * m_low), NextFloatUp(value * m_high));
		}
		else {
			return Interval(NextFloatDown(value * m_high), NextFloatUp(value * m_low));
		}
	}

	Interval Interval::operator/(Theia::Float value) const {
		if (value == 0.0) {
			return Interval(-Theia::Infinity, Theia::Infinity);
		}
		else if (value > 0.0) {
			return Interval(NextFloatDown(m_low / value), NextFloatUp(m_high / value));
		}
		else {
			return Interval(NextFloatDown(m_high / value), NextFloatUp(m_low / value));
		}
	}

	Interval& Interval::operator+=(Theia::Float value) {
		(*this) = (*this) + Interval(value);
		return *this;
	}

	Theia::Float Interval::GetHigh() const {
		return m_high;
	}

	Theia::Float Interval::GetLow() const {
		return m_low;
	}

	Theia::Float Interval::GetMidpoint() const {
		return (m_high + m_low) / 2.0;
	}

	Theia::Float Interval::GetRange() const {
		return m_high - m_low;
	}

	Interval operator+(Theia::Float value, const Interval& interval) {
		return Interval(value) + interval;
	}

	Interval operator-(Theia::Float value, const Interval& interval) {
		return Interval(value) - interval;
	}

	Interval operator*(Theia::Float value, const Interval& interval) {
		if (value > 0.0) {
			return Interval(NextFloatDown(value * interval.GetLow()), NextFloatUp(value * interval.GetHigh()));
		}
		else {
			return Interval(NextFloatDown(value * interval.GetHigh()), NextFloatUp(value * interval.GetLow()));
		}
	}

	Interval operator/(Theia::Float value, const Interval& interval) {

		if (InRange(0.0, interval)) {
			return Interval(-Theia::Infinity, Theia::Infinity);
		}
		else if (value > 0.0) {
			return Interval(NextFloatDown(value / interval.GetHigh()), NextFloatUp(value / interval.GetLow()));
		}
		else {
			return Interval(NextFloatDown(value / interval.GetLow()), NextFloatUp(value / interval.GetHigh()));
		}
	}

	bool InRange(Theia::Float value, const Interval& interval) {
		return value >= interval.GetLow() && value <= interval.GetHigh();
	}

	bool InRange(const Interval& interval1, const Interval& interval2) {
		return interval1.GetLow() <= interval2.GetHigh() && interval1.GetHigh() >= interval2.GetLow();
	}
}
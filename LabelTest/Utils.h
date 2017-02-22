#pragma once

class Utils
{
public:
	static const double DOUBLE_EPSILON;
	static const double M_PI;

	static inline int Round(const double value) { return int(value + 0.5); };

	static inline bool DoubleCompare(
		const double lhs,
		const double rhs,
		const double epsilon = DOUBLE_EPSILON)
	{
		return (fabs(lhs - rhs) < epsilon);
	}

	static inline double DegToRad(const double degrees)
	{
		return degrees * (M_PI / 180.0);
	}

	static inline double RadToDeg(const double radians)
	{
		return radians * (180.0 / M_PI);
	}
}; // namespace Utils

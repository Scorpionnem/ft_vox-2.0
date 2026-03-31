#pragma once

#include "Vec3.hpp"
#include "Vec2.hpp"
#include "Mat4.hpp"

#include "Direction.hpp"
#include "Positions.hpp"

#include <cmath>

struct	Range
{
	Range(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

	float	min;
	float	max;
};

inline float distance_to_range(float v, Range r)
{
	if (v < r.min)
		return (r.min - v);
	if (v > r.max)
		return (v - r.max);
	return (0.0f);
}

inline float radians(float deg)
{
	return (deg * M_PI / 180.0f);
}

inline float mix(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}

template <typename T>
inline Vec3<T> mix(Vec3<T> a, Vec3<T> b, float t)
{
	return Vec3<T>(mix(a.x, b.x, t), mix(a.y, b.y, t), mix(a.z, b.z, t));
}

template <typename T>
inline float	length(const Vec3<T> &v)
{
	return (std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float	frac(float v)
{
	return (v - std::floor(v));
}

template <typename T>
inline Vec3<T>	frac(const Vec3<T> &v)
{
	return (Vec3<T>(frac(v.x), frac(v.y), frac(v.z)));
}

template <typename T>
inline Vec2<T>	frac(const Vec2<T> &v)
{
	return (Vec2<T>(frac(v.x), frac(v.y)));
}

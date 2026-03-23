#pragma once

#include "Vec3.hpp"
#include "Vec2.hpp"
#include "Mat4.hpp"

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

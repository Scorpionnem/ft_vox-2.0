#pragma once

#include <cmath>
#include <cstdint>

template <typename T>
struct	Vec2
{
	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}
	Vec2(T xy) : x(xy), y(xy) {}

	template <typename U>
	Vec2(const Vec2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

	Vec2 operator+(const Vec2 &v) const
	{
		return (Vec2(x + v.x, y + v.y));
	}
	Vec2 operator-(const Vec2 &v) const
	{
		return (Vec2(x - v.x, y - v.y));
	}
	Vec2 operator*(const Vec2 &v) const
	{
		return (Vec2(x * v.x, y * v.y));
	}
	Vec2 operator/(const Vec2 &v) const
	{
		return (Vec2(x / v.x, y / v.y));
	}
	bool operator==(const Vec2 &v) const
	{
		return (x == v.x && y == v.y);
	}

	T	length()
	{
		return (std::sqrt(x * x + y * y));
	}

	T	x, y;
};

using Vec2i = Vec2<int32_t>;
using Vec2u = Vec2<uint32_t>;
using Vec2ul = Vec2<uint64_t>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

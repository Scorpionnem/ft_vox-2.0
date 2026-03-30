#pragma once

#include <cmath>
#include <cstdint>

template <typename T>
struct	Vec2
{
	constexpr Vec2() : x(0), y(0) {}
	constexpr Vec2(T x, T y) : x(x), y(y) {}
	constexpr Vec2(T xy) : x(xy), y(xy) {}

	template <typename U>
	constexpr Vec2(const Vec2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

	constexpr Vec2 operator+(const Vec2 &v) const
	{
		return (Vec2(x + v.x, y + v.y));
	}
	constexpr Vec2 operator-(const Vec2 &v) const
	{
		return (Vec2(x - v.x, y - v.y));
	}
	constexpr Vec2 operator*(const Vec2 &v) const
	{
		return (Vec2(x * v.x, y * v.y));
	}
	constexpr Vec2 operator/(const Vec2 &v) const
	{
		return (Vec2(x / v.x, y / v.y));
	}
	constexpr bool operator==(const Vec2 &v) const
	{
		return (x == v.x && y == v.y);
	}

	constexpr T	length() const
	{
		return (std::sqrt(x * x + y * y));
	}

	T	x, y;
};


template <typename T>
std::ostream &operator<<(std::ostream &s, Vec2<T> vec)
{
	s << "[" << vec.x << " " << vec.y << "]";
	return (s);
}

using Vec2i = Vec2<int32_t>;
using Vec2u = Vec2<uint32_t>;
using Vec2ul = Vec2<uint64_t>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

template<typename T>
T	dot(const Vec2<T> &v1, const Vec2<T> &v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

template<typename T>
Vec2<T>	sin(const Vec2<T> &v)
{
	return (Vec2<T>(std::sin(v.x), std::sin(v.y)));
}

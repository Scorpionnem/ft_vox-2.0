#pragma once

#include <cmath>
#include <cstdint>

template <typename T>
struct	Vec3
{
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vec3(T xyz) : x(xyz), y(xyz), z(xyz) {}

	template <typename U>
	Vec3(const Vec3<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

	Vec3 operator+(const Vec3 &v) const
	{
		return (Vec3(x + v.x, y + v.y, z + v.z));
	}
	Vec3 operator-(const Vec3 &v) const
	{
		return (Vec3(x - v.x, y - v.y, z - v.z));
	}
	Vec3 operator*(const Vec3 &v) const
	{
		return (Vec3(x * v.x, y * v.y, z * v.z));
	}
	Vec3 operator/(const Vec3 &v) const
	{
		return (Vec3(x / v.x, y / v.y, z / v.z));
	}
	bool operator==(const Vec3 &v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	Vec3 operator*(T s) const
	{
		return Vec3(x * s, y * s, z * s);
	}
	Vec3 operator/(T s) const
	{
		return Vec3(x / s, y / s, z / s);
	}

	T	length() const
	{
		return (std::sqrt(x * x + y * y + z * z));
	}

	T	x, y, z;
};

using Vec3i = Vec3<int32_t>;
using Vec3u = Vec3<uint32_t>;
using Vec3ul = Vec3<uint64_t>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

template <typename T>
inline	Vec3<T>	normalize(const Vec3<T> &v)
{
	T	len = v.length();

	return (len > 0.0f ? v / len : Vec3<T>(0));
}

template <typename T>
inline Vec3<T>	cross(const Vec3<T> &v1, const Vec3<T> &v2)
{
	return (Vec3<T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	));
}

template<typename T>
T	dot(const Vec3<T> &v1, const Vec3<T> &v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

template<typename T>
Vec3<T> operator*(T s, const Vec3<T>& v)
{
	return Vec3<T>(v.x * s, v.y * s, v.z * s);
}

#pragma once

#include <cmath>
#include <ostream>
#include <cstdint>

template <typename T>
struct	Vec3
{
	constexpr Vec3() : x(0), y(0), z(0) {}
	constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
	constexpr Vec3(T xyz) : x(xyz), y(xyz), z(xyz) {}

	template <typename U>
	constexpr Vec3(const Vec3<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)) {}

	constexpr Vec3 operator+(const Vec3 &v) const
	{
		return (Vec3(x + v.x, y + v.y, z + v.z));
	}
	constexpr Vec3 operator-(const Vec3 &v) const
	{
		return (Vec3(x - v.x, y - v.y, z - v.z));
	}
	constexpr Vec3 operator*(const Vec3 &v) const
	{
		return (Vec3(x * v.x, y * v.y, z * v.z));
	}
	constexpr Vec3 operator/(const Vec3 &v) const
	{
		return (Vec3(x / v.x, y / v.y, z / v.z));
	}
	constexpr bool operator==(const Vec3 &v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	constexpr Vec3 operator*(T s) const
	{
		return Vec3(x * s, y * s, z * s);
	}
	constexpr Vec3 operator/(T s) const
	{
		return Vec3(x / s, y / s, z / s);
	}

	constexpr T	length() const
	{
		return (std::sqrt(x * x + y * y + z * z));
	}

	constexpr uint64_t	hash() const
	{
		const uint64_t BITS = 21;
		const uint64_t MASK = (1ULL << BITS) - 1;
		const int64_t  BIAS = 1LL << (BITS - 1);

		uint64_t ux = static_cast<uint64_t>(static_cast<int64_t>(x) + BIAS) & MASK;
		uint64_t uy = static_cast<uint64_t>(static_cast<int64_t>(y) + BIAS) & MASK;
		uint64_t uz = static_cast<uint64_t>(static_cast<int64_t>(z) + BIAS) & MASK;

		return (ux << (BITS * 2)) | (uy << (BITS * 1)) | (uz << (BITS * 0));
	}

	T	x, y, z;
};

template <typename T>
std::ostream &operator<<(std::ostream &s, Vec3<T> vec)
{
	s << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
	return (s);
}

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

template<typename T>
Vec3<T>	sin(const Vec3<T> &v)
{
	return (Vec3<T>(std::sin(v.x), std::sin(v.y), std::sin(v.z)));
}

#pragma once

#include <algorithm>

#include "Vec3.hpp"

template <typename T>
struct Mat4
{
	T	data[16];

	Mat4() : Mat4(0.0) {}
	Mat4(T val)
	{
		std::fill(data, data + 16, 0.0f);
		this->data[0] = this->data[5] = this->data[10] = this->data[15] = val;
	}
	Mat4(const Mat4 &cpy)
	{
		*this = cpy;
	}
	Mat4 &operator=(const Mat4 &cpy)
	{
		for (int i = 0; i < 16; i++)
			this->data[i] = cpy.data[i];
		return (*this);
	}
	float &operator()(int col, int row)
	{
		return (data[col * 4 + row]);
	}
	const float &operator()(int col, int row) const
	{
		return (data[col * 4 + row]);
	}
};

using Mat4f = Mat4<float>;

template <typename T>
inline Mat4<T> operator*(const Mat4<T> &a, const Mat4<T> &b)
{
	Mat4<T> result;
	for (int col = 0; col < 4; ++col)
	{
		for (int row = 0; row < 4; ++row)
		{
			result(col, row) = 0.0f;
			for (int k = 0; k < 4; ++k)
				result(col, row) += a(k, row) * b(col, k);
		}
	}
	return (result);
}

template <typename T>
inline Mat4<T> translate(const Vec3<T> &v)
{
	Mat4<T>	m(1);
	m(3, 0) = v.x;
	m(3, 1) = v.y;
	m(3, 2) = v.z;
	return (m);
}

template <typename T>
inline Mat4<T> scale(const Vec3<T> &s)
{
	Mat4<T>	m(1);
	m(0, 0) = s.x;
	m(1, 1) = s.y;
	m(2, 2) = s.z;
	return (m);
}

template <typename T>
inline Mat4<T> rotate(float angleRad, const Vec3<T> &axis)
{
	Vec3<T> a = normalize(axis);
	float c = std::cos(angleRad);
	float s = std::sin(angleRad);
	float oneMinusC = 1.0f - c;

	Mat4<T> m(1);
	m(0, 0) = c + a.x * a.x * oneMinusC;
	m(0, 1) = a.x * a.y * oneMinusC + a.z * s;
	m(0, 2) = a.x * a.z * oneMinusC - a.y * s;

	m(1, 0) = a.y * a.x * oneMinusC - a.z * s;
	m(1, 1) = c + a.y * a.y * oneMinusC;
	m(1, 2) = a.y * a.z * oneMinusC + a.x * s;

	m(2, 0) = a.z * a.x * oneMinusC + a.y * s;
	m(2, 1) = a.z * a.y * oneMinusC - a.x * s;
	m(2, 2) = c + a.z * a.z * oneMinusC;

	return (m);
}

template <typename T>
inline Mat4<T> perspective(float fovDeg, float aspect, float near, float far)
{
	float fovRad = fovDeg * M_PI / 180.0f;
	float f = 1.0f / std::tan(fovRad / 2.0f);

	Mat4<T> m;
	m.data[0] = f / aspect;
	m.data[5] = f;
	m.data[10] = (far + near) / (near - far);
	m.data[11] = -1.0f;
	m.data[14] = (2.0f * far * near) / (near - far);
	return (m);
}

template <typename T>
inline Mat4<T> lookAt(const Vec3<T> &eye, const Vec3<T> &center, const Vec3<T> &up)
{
	Vec3<T> f = normalize(center - eye);
	Vec3<T> s = normalize(cross(f, up));
	Vec3<T> u = cross(s, f);

	Mat4<T> m(1);
	m(0, 0) = s.x; m(1, 0) = s.y; m(2, 0) = s.z;
	m(0, 1) = u.x; m(1, 1) = u.y; m(2, 1) = u.z;
	m(0, 2) = -f.x; m(1, 2) = -f.y; m(2, 2) = -f.z;
	m(3, 0) = -dot(s, eye);
	m(3, 1) = -dot(u, eye);
	m(3, 2) = dot(f, eye);
	return (m);
}

#pragma once

#include "Math.hpp"

inline float rand3dTo1d(Vec3f value, Vec3f dotDir = Vec3f(12.9898, 78.233, 37.719))
{
	Vec3f	smallValue = sin(value);
	float	random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return (random);
}

inline Vec3f rand3dTo3d(Vec3f value)
{
	return Vec3f(
		rand3dTo1d(value, Vec3f(12.989, 78.233, 37.719)),
		rand3dTo1d(value, Vec3f(39.346, 11.135, 83.155)),
		rand3dTo1d(value, Vec3f(73.156, 52.235, 09.151))
	);
}

inline float	rand2dTo1d(Vec2f value, Vec2f dotDir = Vec2f(12.9898, 78.233))
{
	Vec2f smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return (random);
}

inline Vec2f	rand2dTo2d(Vec2f value)
{
	return (Vec2f(
		rand2dTo1d(value, Vec2f(12.989, 78.233)),
		rand2dTo1d(value, Vec2f(39.346, 11.135))
	));
}

inline float	rand1dTo1d(float value, float mutator = 0.546)
{
	return (frac(sin(value + mutator) * 143758.5453));
}

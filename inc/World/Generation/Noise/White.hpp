#pragma once

#include "Math.hpp"

float rand3dTo1d(Vec3f value, Vec3f dotDir = Vec3f(12.9898, 78.233, 37.719)){
    Vec3f	smallValue = sin(value);
    float	random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return (random);
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

float	rand1dTo1d(float value, float mutator = 0.546)
{
	return (frac(sin(value + mutator) * 143758.5453));
}

#pragma once

#include "Math.hpp"
#include "White.hpp"

inline float lerp(float x, float y, float a)
{
	return (x * (1 - a) + y * a);
}

inline float easeIn(float interpolator)
{
	return (interpolator * interpolator);
}

inline float easeOut(float interpolator)
{
	return (1 - easeIn(1 - interpolator));
}

inline float easeInOut(float interpolator)
{
	float	easeInValue = easeIn(interpolator);
	float	easeOutValue = easeOut(interpolator);
	return (lerp(easeInValue, easeOutValue, interpolator));
}

inline float perlin(Vec2f value)
{
	Vec2f	lowerLeftDirection = rand2dTo2d(Vec2f(floor(value.x), floor(value.y))) * 2 - 1;
	Vec2f	lowerRightDirection = rand2dTo2d(Vec2f(ceil(value.x), floor(value.y))) * 2 - 1;
	Vec2f	upperLeftDirection = rand2dTo2d(Vec2f(floor(value.x), ceil(value.y))) * 2 - 1;
	Vec2f	upperRightDirection = rand2dTo2d(Vec2f(ceil(value.x), ceil(value.y))) * 2 - 1;

	Vec2f	fraction = frac(value);

	float	lowerLeftFunctionValue = dot(lowerLeftDirection, fraction - Vec2f(0, 0));
	float	lowerRightFunctionValue = dot(lowerRightDirection, fraction - Vec2f(1, 0));
	float	upperLeftFunctionValue = dot(upperLeftDirection, fraction - Vec2f(0, 1));
	float	upperRightFunctionValue = dot(upperRightDirection, fraction - Vec2f(1, 1));

	float	interpolatorX = easeInOut(fraction.x);
	float	interpolatorY = easeInOut(fraction.y);

	float	lowerCells = lerp(lowerLeftFunctionValue, lowerRightFunctionValue, interpolatorX);
	float	upperCells = lerp(upperLeftFunctionValue, upperRightFunctionValue, interpolatorX);

	float	noise = lerp(lowerCells, upperCells, interpolatorY);
	return (noise);
}

inline float	noise(const Vec2f &pos, float freq, float amp, int noisiness)
{
	float	res = 0;
	for (int i = 0; i < noisiness; i++)
	{
		res += perlin(Vec2f(pos.x * freq, pos.y * freq)) * amp;

		freq *= 2;
		amp /= 2;
	}

	res = std::clamp(res, -1.0f, 1.0f);

	return (res);
}

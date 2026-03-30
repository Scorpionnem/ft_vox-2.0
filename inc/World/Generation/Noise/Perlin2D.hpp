#pragma once

#include "Math.hpp"
#include "White.hpp"

float lerp(float x, float y, float a)
{
	return (x * (1 - a) + y * a);
}

float easeIn(float interpolator)
{
	return (interpolator * interpolator);
}

float easeOut(float interpolator)
{
	return (1 - easeIn(1 - interpolator));
}

float easeInOut(float interpolator)
{
	float	easeInValue = easeIn(interpolator);
	float	easeOutValue = easeOut(interpolator);
	return (lerp(easeInValue, easeOutValue, interpolator));
}

float perlin(Vec2f value)
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

#pragma once

#include "Math.hpp"
#include "Perlin2D.hpp"

inline float	perlin3D(const Vec3f &pos)
{
	float ab = perlin(Vec2f(pos.x, pos.y));
	float bc = perlin(Vec2f(pos.y, pos.z));
	float ac = perlin(Vec2f(pos.x, pos.z));

	return (ab + bc + ac) / 3.0;
}

inline float	noise(const Vec3f &pos, float freq, float amp, int noisiness)
{
	float	res = 0;
	for (int i = 0; i < noisiness; i++)
	{
		res += perlin3D(Vec3f(pos) * freq) * amp;

		freq *= 2;
		amp /= 2;
	}

	res = std::clamp(res, -1.0f, 1.0f);

	return (res);
}

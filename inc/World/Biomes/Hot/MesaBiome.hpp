#pragma once

#include "Biome.hpp"

inline float smoothstep(float edge0, float edge1, float x)
{
	float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return (t * t * (3.0 - 2.0 * t));
}

struct	MesaBiome : public Biome
{
	MesaBiome()
	: Biome(Range(0.85, 1), Range(-1, 1), Range(0, 0), Range(0.33, 1), Range(0, 0))
	{

	}

	// > 0.5 == mesa plateaus
	float			_get_plateauness(const Vec2f &pos)
	{
		return (std::fabs(noise(pos, 0.01, 2, 3)));
	}
	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.02;
		int		height = 20;
		int		min_height = 85;
		int		noisiness = 1;

		float	plateauness = _get_plateauness(pos);

		float	noise1 = noise(Vec2f(pos.x, pos.y), scale, 1, noisiness) * height + min_height;
		float	noise2 = noise(Vec2f(pos.x, pos.y), scale, 1, noisiness) * height + 150;

		float	sharpness = 0.1;
		plateauness = smoothstep(0.5 - sharpness, 0.5 + sharpness, plateauness);
		return (mix(noise1, noise2, plateauness));
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		if (pos.y > world_height)
			return (BLOCK_AIR);
		int	var = static_cast<int>(rand2dTo1d(Vec2i(pos.x, pos.z)) * 3);
		float	p = _get_plateauness(Vec2f(pos.x, pos.z));
		if (p < 0.45 && pos.y == world_height)
			return (BLOCK_SAND);
		if (pos.y >= world_height - (var + 16))
		{
			float	r = rand1dTo1d(pos.y);

			if (r >= 0 && r < 0.125)
				return (BLOCK_TERRACOTA);
			if (r >= 0.125 && r < 0.250)
				return (BLOCK_RED_TERRACOTA);
			if (r >= 0.250 && r < 0.375)
				return (BLOCK_YELLOW_TERRACOTA);
			if (r >= 0.375 && r < 0.500)
				return (BLOCK_BLACK_TERRACOTA);
			if (r >= 0.500 && r < 0.625)
				return (BLOCK_GRAY_TERRACOTA);
			if (r >= 0.625 && r < 0.750)
				return (BLOCK_WHITE_TERRACOTA);
			if (r >= 0.750 && r < 0.875)
				return (BLOCK_ORANGE_TERRACOTA);
			if (r >= 0.875 && r <= 1.0)
				return (BLOCK_BROWN_TERRACOTA);
		}
		return (BLOCK_STONE);
	}
};

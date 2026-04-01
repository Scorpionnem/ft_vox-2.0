#pragma once

#include "Biome.hpp"

struct	GravelBeachBiome : public Biome
{
	GravelBeachBiome()
	: Biome(Range(0.00, 0.1), Range(-1, 0), Range(0, 0), Range(-1, 1), Range(0, 0))
	{

	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.026;
		int		height = 9;
		int		min_height = 62;
		int		noisiness = 2;

		return (noise(Vec2f(pos.x, pos.y), scale, 1, noisiness) * height + min_height);
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		if (pos.y > world_height)
			return (BLOCK_AIR);
		if (pos.y == world_height)
			return (BLOCK_GRAVEL);
		return (BLOCK_STONE);
	}
};

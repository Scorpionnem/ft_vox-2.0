#pragma once

#include "Biome.hpp"

struct	PlateauBiome : public Biome
{
	PlateauBiome()
	: Biome(Range(0.7, 0.85), Range(0, 1), Range(0, 0), Range(-1, 0.33), Range(0, 0))
	{

	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.0078;
		int		height = 8;
		int		min_height = 120;
		int		noisiness = 3;

		return (std::fabs(noise(Vec2f(pos.x, pos.y), scale, 1, noisiness)) * height + min_height);
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		#define SNOW_HEIGHT 200

		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		if (pos.y > world_height)
			return (BLOCK_AIR);
		if (pos.y == world_height)
			return (BLOCK_GRASS);
		return (BLOCK_STONE);
	}
};

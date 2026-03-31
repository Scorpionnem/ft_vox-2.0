#pragma once

#include "Biome.hpp"

struct	MountainLowBiome : public Biome
{
	MountainLowBiome()
	: Biome(Range(0.5, 0.55), Range(0, 0), Range(0, 0), Range(0, 0), Range(0, 0))
	{

	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.008;
		int		height = 20;
		int		min_height = 120;
		int		noisiness = 4;

		return (std::fabs(noise(Vec2f(pos.x, pos.y), scale, 1, noisiness)) * height + min_height);
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		#define SNOW_HEIGHT 200

		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		int	var = static_cast<int>(rand2dTo1d(Vec2i(pos.x, pos.z)) * 8);
		if (pos.y >= SNOW_HEIGHT - (var + 1))
			return (BLOCK_SNOW);
		if (pos.y == world_height)
			return (BLOCK_STONE);
		return (BLOCK_STONE);
	}
};

#pragma once

#include "Biome.hpp"

struct	DeepOceanBiome : public Biome
{
	DeepOceanBiome()
	: Biome(Range(-1, -0.7), Range(-1, 1), Range(0, 0), Range(-1, 1), Range(0, 0))
	{

	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.0123;
		int		height = 8;
		int		min_height = -8;
		int		noisiness = 4;

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
		int	var = static_cast<int>(rand2dTo1d(Vec2i(pos.x, pos.z)) * 3);
		if (pos.y >= world_height - (var + 1))
			return (BLOCK_GRAVEL);
		return (BLOCK_STONE);
	}
};

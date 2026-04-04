#pragma once

#include "Biome.hpp"

struct	ShallowOceanBiome : public Biome
{
	ShallowOceanBiome()
	: Biome(Range(-0.2, 0.00), Range(-1, 1), Range(0, 0), Range(-1, 1), Range(0, 0))
	{

	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.0165;
		int		height = 6;
		int		min_height = 53;
		int		noisiness = 2;

		return (noise(Vec2f(pos.x, pos.y), scale, 1, noisiness) * height + min_height);
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		if (pos.y == WATER_LEVEL + 1 && rand2dTo1d(Vec2f(pos.x, pos.z)) <= 0.005)
			return (BLOCK_LILYPAD);
		if (pos.y > world_height)
			return (BLOCK_AIR);
		if (pos.y == world_height)
			return (BLOCK_SAND);
		int	var = static_cast<int>(rand2dTo1d(Vec2i(pos.x, pos.z)) * 3);
		if (pos.y >= world_height - (var + 1))
			return (BLOCK_SANDSTONE);
		return (BLOCK_STONE);
	}
};

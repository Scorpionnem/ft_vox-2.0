#pragma once

#include "Biome.hpp"

struct	ForestBiome : public Biome
{
	ForestBiome()
	: Biome(Range(0.1, 0.7), Range(-1, 0), Range(0, 0), Range(-1, 0.33), Range(0, 0))
	{
		tree_probab = 1;
	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.04;
		int		height = 20;
		int		min_height = 85;
		int		noisiness = 1;

		return (noise(Vec2f(pos.x, pos.y), scale, 1, noisiness) * height + min_height);
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		if (pos.y == world_height + 1 && rand2dTo1d(Vec2f(pos.x, pos.z)) > 0.9)
			return (BLOCK_TALL_GRASS);
		if (pos.y == world_height + 1 && rand2dTo1d(Vec2f(pos.x, pos.z), Vec2f(12.9898, 48.233)) > 0.97)
			return (BLOCK_ROSE);
		if (pos.y == world_height + 1 && rand2dTo1d(Vec2f(pos.x, pos.z), Vec2f(15.9898, 78.233)) > 0.97)
			return (BLOCK_DANDELION);
		if (pos.y > world_height)
			return (BLOCK_AIR);
		if (pos.y == world_height)
			return (BLOCK_GRASS);
		int	var = static_cast<int>(rand2dTo1d(Vec2i(pos.x, pos.z)) * 3);
		if (pos.y >= world_height - (var + 1))
			return (BLOCK_DIRT);
		return (BLOCK_STONE);
	}
};

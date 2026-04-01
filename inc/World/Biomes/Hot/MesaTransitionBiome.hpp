#pragma once

#include "Biome.hpp"

struct	MesaTransitionBiome : public Biome
{
	MesaTransitionBiome()
	: Biome(Range(0.75, 0.85), Range(-1, 0), Range(0, 0), Range(0.33, 1), Range(0, 0))
	{

	}

	float			get_height(const Vec2i &pos)
	{
		float	scale = 0.008;
		int		height = 8;
		int		min_height = 90;
		int		noisiness = 5;

		return (std::fabs(noise(Vec2f(pos.x, pos.y), scale, 1, noisiness)) * height + min_height);
	}
	BlockStateId	get_block(WorldVec3i pos, int world_height)
	{
		if (pos.y <= WATER_LEVEL && pos.y > world_height)
			return (BLOCK_WATER);
		if (pos.y > world_height)
			return (BLOCK_AIR);
		int	var = static_cast<int>(rand2dTo1d(Vec2i(pos.x, pos.z)) * 3);
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

#pragma once

#include "Structure.hpp"
#include "Noise.hpp"

class	Sugarcane : public Structure
{
	public:
		Sugarcane() {}
		~Sugarcane() {}

		std::vector<StructureBlock>	generate(WorldVec3i gen_pos)
		{
			std::vector<StructureBlock>	res;

			if (gen_pos.y <= WATER_LEVEL)
				return (res);

			#define MIN_SUGARCANE_HEIGHT 2
			#define MAX_SUGARCANE_HEIGHT 20
			int	height_var = MIN_SUGARCANE_HEIGHT + static_cast<int>(rand3dTo1d(gen_pos) * (float)MAX_SUGARCANE_HEIGHT);

			res.push_back({gen_pos, BLOCK_SAND});

			WorldVec3i	block_pos = gen_pos;
			for (block_pos.y = gen_pos.y + 1; block_pos.y <= gen_pos.y + height_var; block_pos.y++)
				res.push_back({block_pos, BLOCK_SUGARCANE});

			return (res);
		}
};

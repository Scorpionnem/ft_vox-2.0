#pragma once

#include "Structure.hpp"
#include "Noise.hpp"

class	DesertPyramid : public Structure
{
	public:
		DesertPyramid() {}
		~DesertPyramid() {}

		std::vector<StructureBlock>	generate(WorldVec3i gen_pos)
		{
			std::vector<StructureBlock>	res;

			#define MAX_DEPTH 8
			#define MIN_DEPTH 2
			#define PYRAMID_HEIGHT 16
			int	depth_var = MIN_DEPTH + static_cast<int>(rand3dTo1d(gen_pos) * (float)MAX_DEPTH);

			gen_pos.y -= depth_var;

			WorldVec3i	block_pos = gen_pos;

			for (block_pos.y = gen_pos.y; block_pos.y <= gen_pos.y + PYRAMID_HEIGHT; block_pos.y++)
			{
				int height = block_pos.y - gen_pos.y;
				int layer_size = PYRAMID_HEIGHT - height;
				for (block_pos.x = gen_pos.x - layer_size / 2; block_pos.x <= gen_pos.x + layer_size / 2; block_pos.x++)
					for (block_pos.z = gen_pos.z - layer_size / 2; block_pos.z <= gen_pos.z + layer_size / 2; block_pos.z++)
						res.push_back({block_pos, BLOCK_SANDSTONE});
			}

			return (res);
		}
};

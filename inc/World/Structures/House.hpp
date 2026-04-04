#pragma once

#include "Structure.hpp"

class	House : public Structure
{
	public:
		House() {}
		~House() {}

		std::vector<StructureBlock>	generate(WorldVec3i gen_pos)
		{
			std::vector<StructureBlock>	res;

			if (gen_pos.y <= WATER_LEVEL)
				return (res);

			WorldVec3i	block_pos;

			#define CUBE_SIZE 8
			#define CUBE_SIZE_Z 10
			for (block_pos.y = gen_pos.y; block_pos.y < gen_pos.y + CUBE_SIZE; block_pos.y++)
				for (block_pos.x = gen_pos.x - CUBE_SIZE / 2; block_pos.x < gen_pos.x + CUBE_SIZE / 2; block_pos.x++)
					for (block_pos.z = gen_pos.z - CUBE_SIZE_Z / 2; block_pos.z < gen_pos.z + CUBE_SIZE_Z / 2; block_pos.z++)
					{
						if (block_pos.x == (gen_pos.x + CUBE_SIZE / 2) - 1
							|| block_pos.y == gen_pos.y + CUBE_SIZE - 1
							|| block_pos.z == (gen_pos.z + CUBE_SIZE_Z / 2) - 1
							|| block_pos.x == gen_pos.x - CUBE_SIZE / 2
							|| block_pos.y == gen_pos.y
							|| block_pos.z == gen_pos.z - CUBE_SIZE_Z / 2)
							res.push_back({block_pos, BLOCK_BRICKS});
					}

			return (res);
		}
};

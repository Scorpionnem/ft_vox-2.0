#pragma once

#include "Structure.hpp"

class	SmallOakTree : public Structure
{
	public:
		SmallOakTree() {}
		~SmallOakTree() {}

		std::vector<StructureBlock>	generate(WorldVec3i gen_pos)
		{
			std::vector<StructureBlock>	res;

			if (gen_pos.y <= WATER_LEVEL)
				return (res);

			WorldVec3i	block_pos;
			for (block_pos.x = gen_pos.x - 2; block_pos.x <= gen_pos.x + 2; block_pos.x++)
				for (block_pos.z = gen_pos.z - 2; block_pos.z <= gen_pos.z + 2; block_pos.z++)
					for (block_pos.y = gen_pos.y + 4; block_pos.y <= gen_pos.y + 8; block_pos.y++)
						res.push_back({block_pos, BLOCK_OAK_LEAVES});
			block_pos = gen_pos;
			for (block_pos.y = gen_pos.y; block_pos.y <= gen_pos.y + 7; block_pos.y++)
				res.push_back({block_pos, BLOCK_OAK_LOG});

			return (res);
		}
};

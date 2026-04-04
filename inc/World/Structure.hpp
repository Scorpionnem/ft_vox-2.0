#pragma once

#include "BlockType.hpp"
#include "Math.hpp"

struct	StructureBlock
{
	StructureBlock(WorldVec3i pos, BlockStateId block) : pos(pos), block(block)
	{}
	WorldVec3i		pos;
	BlockStateId	block;
};

class	Structure
{
	public:
		Structure() {}
		virtual ~Structure() {}

		virtual std::vector<StructureBlock>	generate(WorldVec3i gen_pos) = 0;
	private:
};

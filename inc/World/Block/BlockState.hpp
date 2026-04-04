#pragma once

#include <cstdint>
#include <string>

using BlockStateId = uint32_t;
using BlockStateHash = uint32_t;
/* DONT forget to implement local block state table when implementing blockstates */
using ChunkBlockStateId = 	uint16_t;

#define BLOCK_TERRACOTA 221
#define BLOCK_BLACK_TERRACOTA 220
#define BLOCK_BROWN_TERRACOTA 219
#define BLOCK_ORANGE_TERRACOTA 218
#define BLOCK_RED_TERRACOTA 217
#define BLOCK_GRAY_TERRACOTA 233
#define BLOCK_WHITE_TERRACOTA 234
#define BLOCK_YELLOW_TERRACOTA 235

#define BLOCK_COBBLESTONE 16
#define BLOCK_SANDSTONE 208
#define BLOCK_BEDROCK 17
#define BLOCK_GRAVEL 19
#define BLOCK_GRASS 40
#define BLOCK_OAK_LEAVES 52
#define BLOCK_OAK_LOG 20
#define BLOCK_BRICKS 7
#define BLOCK_TALL_GRASS 39
#define BLOCK_DEAD_BUSH 55
#define BLOCK_SNOW 66
#define BLOCK_STONE 1
#define BLOCK_SAND 18
#define BLOCK_DIRT 2
#define BLOCK_AIR 0
#define BLOCK_WATER 223
#define BLOCK_ICE 67
#define BLOCK_CACTUS 70
#define BLOCK_CLAY 72

#define BLOCK_ROSE 12
#define BLOCK_DANDELION 13

#define WATER_LEVEL 60

class	BlockType;

class	BlockState
{
	public:
		BlockState(BlockType *parent, BlockStateHash properties)
		{
			_parent = parent;
			_properties = properties;
			_id = _globalId++;
		}

		uint8_t	getProperty(const std::string &prop);

		BlockStateId	id() {return (_id);}
		BlockType		*getParent() {return (_parent);}

		static BlockStateId	getGlobalId() {return (_globalId);}
	private:
		BlockType					*_parent;

		BlockStateHash				_properties;

		BlockStateId				_id;
		static BlockStateId			_globalId;
};

#pragma once

#include <cstdint>
#include <cmath>

struct	BlockProperty
{
	public:
		constexpr BlockProperty(const char *name, uint8_t minVal, uint8_t maxVal)
		: name(name)
		{
			this->minVal = minVal;
			this->maxVal = maxVal;
			bitCount = (int)(log(maxVal) / log(2) + 1);
		}

		const char	*name;

		uint8_t		minVal = 0;
		uint8_t		maxVal = 0;

		uint8_t		bitCount = 0;
};

namespace BlockProperties
{
	const BlockProperty	FACING("FACING", 0, 5);

	const BlockProperty	LIT("LIT", false, true);

	const BlockProperty	WATERLOGGED("WATERLOGGED", false, true);

	/* Age of the block (For plants?) */
	const BlockProperty	AGE("AGE", 0, 32);

	/* What axis the block is oriented to. (x, y, z) */
	const BlockProperty	AXIS("AXIS", 0, 2);
};

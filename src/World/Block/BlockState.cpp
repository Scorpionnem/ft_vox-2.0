#include "BlockState.hpp"
#include "BlockType.hpp"

static uint8_t	_bitMask(uint8_t n)
{
	return ((1u << n) - 1u);
}

BlockStateId	BlockState::_globalId = 0;

uint8_t	BlockState::getProperty(const std::string &prop)
{
	return ((_properties >> _parent->offsetOf(prop)) & _bitMask(_parent->sizeOf(prop)));
}

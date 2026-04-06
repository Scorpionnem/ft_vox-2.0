#include "Math.hpp"
#include "World.hpp"

bool	is_inside(Vec3f pos_a, Vec3f size_a, Vec3f pos_b, Vec3f size_b)
{
	return (
		pos_a.x < pos_b.x + size_b.x &&
		pos_a.x + size_a.x > pos_b.x &&
		pos_a.y < pos_b.y + size_b.y &&
		pos_a.y + size_a.y > pos_b.y &&
		pos_a.z < pos_b.z + size_b.z &&
		pos_a.z + size_a.z > pos_b.z
	);
}

bool	_collides_with_block(Vec3f pos, Vec3f size, Vec3i block_pos, World &world)
{
	if (is_inside(pos, size, block_pos, Vec3f(1)))
	{
		auto	chunk = world.getChunk(worldToChunkWorld(block_pos, CHUNK_SIZE));
		if (chunk && chunk->state() >= Chunk::State::GENERATED)
		{
			BlockStateId block = chunk->getBlock(worldToChunkLocal(block_pos, CHUNK_SIZE));
			if (block != BLOCK_AIR && block != BLOCK_TALL_GRASS && block != BLOCK_DEAD_BUSH && block != BLOCK_ROSE && block != BLOCK_DANDELION && block != BLOCK_SUGARCANE)
				return (true);
		}
	}
	return (false);
}

bool	_collides_with_world(Vec3f pos, Vec3f size, World &world)
{
	Vec3i	corner1_block = block_pos(pos);
	Vec3i	corner2_block = block_pos(Vec3f(pos.x + size.x, pos.y, pos.z));
	Vec3i	corner3_block = block_pos(Vec3f(pos.x, pos.y + size.y, pos.z));
	Vec3i	corner4_block = block_pos(Vec3f(pos.x, pos.y, pos.z + size.z));
	Vec3i	corner5_block = block_pos(Vec3f(pos.x + size.x, pos.y + size.y, pos.z));
	Vec3i	corner7_block = block_pos(Vec3f(pos.x + size.x, pos.y, pos.z + size.z));
	Vec3i	corner8_block = block_pos(Vec3f(pos.x, pos.y + size.y, pos.z + size.z));
	Vec3i	corner6_block = block_pos(Vec3f(pos.x + size.x, pos.y + size.y, pos.z + size.z));

	return (_collides_with_block(pos, size, corner1_block, world)
			|| _collides_with_block(pos, size, corner2_block, world)
			|| _collides_with_block(pos, size, corner3_block, world)
			|| _collides_with_block(pos, size, corner4_block, world)
			|| _collides_with_block(pos, size, corner5_block, world)
			|| _collides_with_block(pos, size, corner6_block, world)
			|| _collides_with_block(pos, size, corner7_block, world)
			|| _collides_with_block(pos, size, corner8_block, world));
}

Vec3f	_solve_collisions(Vec3f pos, Vec3f size, Vec3f velocity, World &world)
{
	pos.x += velocity.x;
	if (_collides_with_world(pos - (size / 2.0), size, world))
	{
		pos.x -= velocity.x;
		velocity.x = 0;
	}
	pos.y += velocity.y;
	if (_collides_with_world(pos - (size / 2.0), size, world))
	{
		pos.y -= velocity.y;
		velocity.y = 0;
	}
	pos.z += velocity.z;
	if (_collides_with_world(pos - (size / 2.0), size, world))
	{
		pos.z -= velocity.z;
		velocity.z = 0;
	}
	return (velocity);
}

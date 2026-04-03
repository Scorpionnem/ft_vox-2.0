#pragma once

#include "Vec3.hpp"

/* Position in chunk local space (0-CHUNK_SIZE) */
using ChunkLocalVec3i	= 	Vec3i;
/* Position in chunk world space */
using ChunkWorldVec3i	= 	Vec3i;
/* Position in world space */
using WorldVec3i		= 	Vec3i;

/* Position in chunk local space (0-CHUNK_SIZE) */
using ChunkLocalVec3f	= 	Vec3f;
/* Position in chunk world space */
using ChunkWorldVec3f	= 	Vec3f;
/* Position in world space */
using WorldVec3f		= 	Vec3f;

inline int	floorDiv(int x, int d)
{
	return (x >= 0) ? (x / d) : ((x - d + 1) / d);
}

inline int	floorMod(int x, int d)
{
	int m = x % d;
	return (m < 0) ? (m + d) : m;
}

inline ChunkWorldVec3i	worldToChunkWorld(const ChunkLocalVec3i &pos, int chunk_size)
{
	return ChunkWorldVec3i(
		floorDiv(pos.x, chunk_size),
		floorDiv(pos.y, chunk_size),
		floorDiv(pos.z, chunk_size)
	);
}
inline WorldVec3i	chunkLocalToWorld(const ChunkLocalVec3i &pos, const ChunkWorldVec3i &chunk_pos, int chunk_size)
{
	return (pos + (chunk_pos * chunk_size));
}

inline ChunkLocalVec3i	worldToChunkLocal(const WorldVec3i &pos, int chunk_size)
{
	return ChunkLocalVec3i(
		floorMod(pos.x, chunk_size),
		floorMod(pos.y, chunk_size),
		floorMod(pos.z, chunk_size)
	);
}

#pragma once

#include <unordered_map>
#include <memory>

#include "Chunk.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "BlockType.hpp"

using ChunkPosHash = uint64_t;

class	World
{
	public:
		World() {}
		~World() {}

		void	imgui();

		// Updates all chunks around update centers (players) Generates chunks if needed giving them to the given thread pool
		void	update(ThreadPool &genThreads, double delta);

		// Gets all chunks in a camera's view
		std::vector<std::shared_ptr<Chunk>>	getVision(const Camera &cam, const Vec3i &viewDistance, bool cull);

		void	setUpdateCenter(const WorldVec3i &pos)
		{
			_updateCenter = worldToChunkWorld(pos,CHUNK_SIZE);
		}

		std::shared_ptr<Chunk>	getChunk(const ChunkWorldVec3i &pos)
		{
			std::unique_lock<std::mutex>	lock(_chunksMutex);

			ChunkPosHash	hash = pos.hash();

			auto find = _chunks.find(hash);

			if (find == _chunks.end())
				return (NULL);
			return (find->second);
		}
		BlockStateId	getBlock(const WorldVec3i &pos)
		{
			std::shared_ptr<Chunk>	chunk = getChunk(worldToChunkWorld(pos, CHUNK_SIZE));
			if (chunk && chunk->state() >= Chunk::State::GENERATED)
				return (chunk->getBlock(worldToChunkLocal(pos, CHUNK_SIZE)));
			throw std::runtime_error("Failed to get block (Chunk not loaded)");
		}
		void	setBlock(const WorldVec3i &pos, BlockStateId block)
		{
			std::shared_ptr<Chunk>	chunk = getChunk(worldToChunkWorld(pos, CHUNK_SIZE));
			if (chunk && chunk->state() >= Chunk::State::GENERATED)
			{
				chunk->setBlock(worldToChunkLocal(pos, CHUNK_SIZE), block);
				if (chunk->state() >= Chunk::State::MESHED)
				{
					chunk->mesh();
					chunk->mesh_neighbours();
				}
				return ;
			}
			throw std::runtime_error("Failed to set block (Chunk not loaded)");
		}
		void	breakBlock(const WorldVec3i &pos)
		{
			setBlock(pos, BLOCK_AIR);
		}

		void	castRayToBlock(const WorldVec3f &pos, const Vec3f &dir, int MAX_STEPS, bool &hit, Vec3i &hit_pos, Vec3i &prev_hit_pos);

		void	setUpdateDistance(const Vec3i &dist)
		{
			_updateDistance = dist;
		}
	private:
		void	_moveRay(Vec3i &mapPos, Vec3f &sideDist, const Vec3f &deltaDist, const Vec3i &rayStep);

		std::shared_ptr<Chunk>	_addChunk(const ChunkWorldVec3i &pos)
		{
			std::unique_lock<std::mutex>	lock(_chunksMutex);

			std::shared_ptr<Chunk>	chunk = std::make_shared<Chunk>(pos, this);

			_chunks.insert(std::make_pair(pos.hash(), chunk));
			return (chunk);
		}
		std::unordered_map<ChunkPosHash, std::shared_ptr<Chunk>>	_chunks;

		ChunkWorldVec3i	_updateCenter;
		Vec3i			_updateDistance = Vec3i(9);

		std::mutex	_chunksMutex;
};

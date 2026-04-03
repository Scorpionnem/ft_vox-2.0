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
		std::vector<std::shared_ptr<Chunk>>	getVision(const Camera &cam, const Vec3i &viewDistance);

		void	setUpdateCenter(const WorldVec3i &pos)
		{
			_updateCenter = pos / CHUNK_SIZE;
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

		void	castRayToBlock(const WorldVec3f &pos, const Vec3f &dir, int MAX_STEPS, bool &hit, Vec3i &hit_pos, Vec3i &prev_hit_pos)
		{
				prev_hit_pos = floor(pos);
				hit_pos = floor(pos);
				hit = false;

				Vec3f	deltaDist = abs(Vec3f(1.0f) / dir);
				Vec3f	sideDist = (sign(dir) * (Vec3f(hit_pos) - pos) + (sign(dir) * 0.5f) + 0.5f) * deltaDist;

				Vec3i	rayStep = Vec3i(sign(dir));

				for (int i = 0; i < MAX_STEPS; ++i)
				{
					prev_hit_pos = hit_pos;
					moveRay(hit_pos, sideDist, deltaDist, rayStep);
					auto chunk = getChunk(worldToChunkWorld(hit_pos, CHUNK_SIZE));
					if (!chunk || chunk->state() < Chunk::State::GENERATED)
						continue ;
					if (chunk->getBlock(worldToChunkLocal(hit_pos, CHUNK_SIZE)) != BLOCK_AIR)
					{
						hit = true;
						break ;
					}
				}
		}
		void	moveRay(Vec3i &mapPos, Vec3f &sideDist, const Vec3f &deltaDist, const Vec3i &rayStep)
		{
			if (sideDist.x < sideDist.y)
			{
				if (sideDist.x < sideDist.z)
				{
					sideDist.x += deltaDist.x;
					mapPos.x += rayStep.x;
				}
				else
				{
					sideDist.z += deltaDist.z;
					mapPos.z += rayStep.z;
				}
			}
			else
			{
				if (sideDist.y < sideDist.z)
				{
					sideDist.y += deltaDist.y;
					mapPos.y += rayStep.y;
				}
				else
				{
					sideDist.z += deltaDist.z;
					mapPos.z += rayStep.z;
				}
			}
		}
	private:
		std::shared_ptr<Chunk>	_addChunk(const ChunkWorldVec3i &pos)
		{
			std::unique_lock<std::mutex>	lock(_chunksMutex);

			std::shared_ptr<Chunk>	chunk = std::make_shared<Chunk>(pos, this);

			_chunks.insert(std::make_pair(pos.hash(), chunk));
			return (chunk);
		}
		std::unordered_map<ChunkPosHash, std::shared_ptr<Chunk>>	_chunks;

		ChunkWorldVec3i	_updateCenter;
		Vec3i			_updateDistance = Vec3i(8, 8, 8);

		std::mutex	_chunksMutex;
};

#pragma once

#include <unordered_map>
#include <memory>

#include "Chunk.hpp"
#include "ThreadPool.hpp"

using ChunkPosHash = uint64_t;

class	World
{
	public:
		World() {}
		~World() {}

		void	update(ThreadPool &genThreads, double delta)
		{
			ChunkWorldVec3i	updateStart = _updateCenter + _updateDistance;
			ChunkWorldVec3i	updateEnd = _updateCenter - _updateDistance;

			std::vector<std::function<void(void)>>	tasks;

			ChunkWorldVec3i	chunkPos;
			for (chunkPos.y = updateStart.y; chunkPos.y >= updateEnd.y; chunkPos.y--)
				for (chunkPos.x = updateStart.x; chunkPos.x >= updateEnd.x; chunkPos.x--)
					for (chunkPos.z = updateStart.z; chunkPos.z >= updateEnd.z; chunkPos.z--)
					{
						std::shared_ptr<Chunk>	chunk = getChunk(chunkPos);

						if (!chunk)
						{
							chunk = _addChunk(chunkPos);
							tasks.push_back([chunk](){chunk->generate();});
							continue ;
						}

						if (chunk->getState() < Chunk::State::GENERATED)
							continue ;

						chunk->update(delta);
					}
			genThreads.queue_task(tasks);
		}
		std::vector<std::shared_ptr<Chunk>>	getVision(const Camera &cam, const Vec3i &viewDistance)
		{
			std::vector<std::shared_ptr<Chunk>>	res;

			ChunkWorldVec3i	viewCenter = cam.pos / CHUNK_SIZE;

			ChunkWorldVec3i	viewStart = viewCenter + viewDistance;
			ChunkWorldVec3i	viewEnd = viewCenter - viewDistance;

			ChunkWorldVec3i	chunkPos;
			for (chunkPos.y = viewStart.y; chunkPos.y >= viewEnd.y; chunkPos.y--)
				for (chunkPos.x = viewStart.x; chunkPos.x >= viewEnd.x; chunkPos.x--)
					for (chunkPos.z = viewStart.z; chunkPos.z >= viewEnd.z; chunkPos.z--)
					{
						std::shared_ptr<Chunk>	chunk = getChunk(chunkPos);

						if (chunk == nullptr || chunk->getState() < Chunk::State::MESHED)
							continue ;

						if (cam.frustum.isInside(Vec3(chunk->getPos() * CHUNK_SIZE), Vec3(chunk->getPos() * CHUNK_SIZE + CHUNK_SIZE)))
							res.push_back(chunk);
					}
			return (res);
		}

		void	setUpdateCenter(const WorldVec3i &pos)
		{
			_updateCenter = pos / CHUNK_SIZE;
		}

		std::shared_ptr<Chunk>	getChunk(const ChunkWorldVec3i &pos)
		{
			ChunkPosHash	hash = pos.hash();

			auto find = _chunks.find(hash);

			if (find == _chunks.end())
				return (NULL);
			return (find->second);
		}
	private:
		std::shared_ptr<Chunk>	_addChunk(const ChunkWorldVec3i &pos)
		{
			std::shared_ptr<Chunk>	chunk = std::make_shared<Chunk>(pos);

			_chunks.insert(std::make_pair(pos.hash(), chunk));
			return (chunk);
		}
		std::unordered_map<ChunkPosHash, std::shared_ptr<Chunk>>	_chunks;

		ChunkWorldVec3i	_updateCenter;
		Vec3i			_updateDistance = Vec3i(4, 4, 4);
};

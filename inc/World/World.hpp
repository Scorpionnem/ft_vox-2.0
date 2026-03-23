#pragma once

#include <unordered_map>
#include <memory>

#include "Chunk.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"

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
		Vec3i			_updateDistance = Vec3i(4, 4, 4);

		std::mutex	_chunksMutex;
};

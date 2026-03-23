#include "World.hpp"

void	World::update(ThreadPool &genThreads, double delta)
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

std::vector<std::shared_ptr<Chunk>>	World::getVision(const Camera &cam, const Vec3i &viewDistance)
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

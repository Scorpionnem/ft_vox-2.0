#include "World.hpp"
#include "Chrono.hpp"

#include <imgui.h>

void	World::imgui()
{
	if (ImGui::Begin("World"))
	{
		ImGui::Text("Chunks: %ld", _chunks.size());
		ImGui::InputInt3("Update distance", &_updateDistance.x);
	}
	ImGui::End();
}

void	World::update(ThreadPool &genThreads, double delta)
{
	ChunkWorldVec3i	updateStart = _updateCenter + _updateDistance;
	ChunkWorldVec3i	updateEnd = _updateCenter - _updateDistance;

	std::vector<std::shared_ptr<Chunk>>	chunks_to_gen;

	for (auto &[hash, chunk] : _chunks)
		chunk->check = false;

	ChunkWorldVec3i	chunkPos;
	for (chunkPos.y = updateStart.y; chunkPos.y >= updateEnd.y; chunkPos.y--)
		for (chunkPos.x = updateStart.x; chunkPos.x >= updateEnd.x; chunkPos.x--)
			for (chunkPos.z = updateStart.z; chunkPos.z >= updateEnd.z; chunkPos.z--)
			{
				std::shared_ptr<Chunk>	chunk = getChunk(chunkPos);

				if (!chunk)
				{
					chunk = _addChunk(chunkPos);
					chunks_to_gen.push_back(chunk);
					continue ;
				}

				chunk->check = true;

				if (chunk->state() < Chunk::State::GENERATED)
					continue ;

				chunk->update(delta);
			}
	for (auto it = _chunks.begin(); it != _chunks.end();)
	{
		std::shared_ptr<Chunk>	chunk = it->second;

		if (!chunk->check)
		{
			if (chunk->edited())
				chunk->save(chunk->get_chunk_path(chunk->pos()));
			it = _chunks.erase(it);
		}
		else
			it++;
	}

	std::sort(chunks_to_gen.begin(), chunks_to_gen.end(),
	[this](std::shared_ptr<Chunk> c1, std::shared_ptr<Chunk> c2)
	{
		return (length(_updateCenter - ((c1->pos() * CHUNK_SIZE) + CHUNK_SIZE / 2)) < length(_updateCenter - ((c2->pos() * CHUNK_SIZE) + CHUNK_SIZE / 2)));
	});

	std::vector<std::function<void(void)>>	tasks;
	for (std::shared_ptr<Chunk> chunk : chunks_to_gen)
		tasks.push_back([chunk](){chunk->generate();});
	genThreads.queue_task(tasks);
}

std::vector<std::shared_ptr<Chunk>>	World::getVision(const Camera &cam, const Vec3i &viewDistance)
{
	std::vector<std::shared_ptr<Chunk>>	res;

	ChunkWorldVec3i	viewCenter = worldToChunkWorld(cam.pos, CHUNK_SIZE);

	ChunkWorldVec3i	viewStart = viewCenter + viewDistance;
	ChunkWorldVec3i	viewEnd = viewCenter - viewDistance;

	ChunkWorldVec3i	chunkPos;
	for (chunkPos.y = viewStart.y; chunkPos.y >= viewEnd.y; chunkPos.y--)
		for (chunkPos.x = viewStart.x; chunkPos.x >= viewEnd.x; chunkPos.x--)
			for (chunkPos.z = viewStart.z; chunkPos.z >= viewEnd.z; chunkPos.z--)
			{
				std::shared_ptr<Chunk>	chunk = getChunk(chunkPos);

				if (chunk == nullptr || chunk->state() < Chunk::State::MESHED || !chunk->has_visible_faces())
					continue ;

				if (cam.frustum.isInside(Vec3(chunk->pos() * CHUNK_SIZE) - cam.pos, Vec3(chunk->pos() * CHUNK_SIZE + CHUNK_SIZE)  - cam.pos))
					res.push_back(chunk);
			}

	std::sort(res.begin(), res.end(),
	[&cam](std::shared_ptr<Chunk> c1, std::shared_ptr<Chunk> c2)
	{
		return (length(cam.pos - ((c1->pos() * CHUNK_SIZE) + CHUNK_SIZE / 2)) > length(cam.pos - ((c2->pos() * CHUNK_SIZE) + CHUNK_SIZE / 2)));
	});
	return (res);
}

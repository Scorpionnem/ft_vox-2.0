#include "Chunk.hpp"
#include "World.hpp"

#include <iostream>

constexpr const Vec3i	DIR_OFFSET[6] =
{
	Vec3i(0, 1, 0), // TOP
	Vec3i(0, -1, 0), // BOTTOM
	Vec3i(0, 0, 1), // NORTH
	Vec3i(0, 0, -1), // SOUTH
	Vec3i(1, 0, 0), // EAST
	Vec3i(-1, 0, 0), // WEST
};

void	Chunk::generate(/*Generator *gen*/)
{
	_blocks.resize(CHUNK_VOLUME);

	ChunkLocalVec3i	pos;

	for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
		for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y++)
			for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
			{
				WorldVec3i	wp = pos + (_pos * CHUNK_SIZE);

				int	y = (sin(wp.x / 8.0) + sin(wp.z / 8.0)) * 16;

				if (wp.y < y)
					setBlock(pos, 1);
				else
					setBlock(pos, 0);
			}
	_state = Chunk::State::GENERATED;
	for (int dir = 0; dir < 6; dir++)
	{
		std::shared_ptr<Chunk> chunk = _world->getChunk(_pos + DIR_OFFSET[dir]);
		if (chunk && chunk->getState() >= Chunk::State::MESHED)
			chunk->mesh();
	}
	mesh();
}

#include <glad/glad.h>

void	Chunk::upload()
{
	if (!_chunkMutex.try_lock())
		return ;

	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	if (VBO != 0)
		glDeleteBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _mesh.size() * sizeof(Vertex), _mesh.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	_state = Chunk::State::UPLOADED;
	_mesh_size = _mesh.size();
	_mesh.clear();

	_chunkMutex.unlock();
}

constexpr const Vec2f	UV00(0.f, 0.f);
constexpr const Vec2f	UV10(1.f, 0.f);
constexpr const Vec2f	UV11(1.f, 1.f);
constexpr const Vec2f	UV01(0.f, 1.f);
constexpr const Vec3f	V1(0, 1, 1);
constexpr const Vec3f	V2(1, 1, 1);
constexpr const Vec3f	V3(1, 1, 0);
constexpr const Vec3f	V4(0, 1, 0);
constexpr const Vec3f	V5(0, 0, 1);
constexpr const Vec3f	V6(1, 0, 1);
constexpr const Vec3f	V7(1, 0, 0);
constexpr const Vec3f	V8(0, 0, 0);

constexpr const Face	FACE1[6] =
{
	Face( // TOP
		Vertex(V1, Vec3f(0,1,0), UV01),
		Vertex(V2, Vec3f(0,1,0), UV11),
		Vertex(V4, Vec3f(0,1,0), UV00)
	),
	Face( // BOTTOM
		Vertex(V8, Vec3f(0,-1,0), UV01),
		Vertex(V6, Vec3f(0,-1,0), UV10),
		Vertex(V5, Vec3f(0,-1,0), UV00)
	),
	Face( // NORTH
		Vertex(V5, Vec3f(0,0,1), UV00),
		Vertex(V2, Vec3f(0,0,1), UV11),
		Vertex(V1, Vec3f(0,0,1), UV01)
	),
	Face( // SOUTH
		Vertex(V8, Vec3f(0,0,-1), UV00),
		Vertex(V4, Vec3f(0,0,-1), UV01),
		Vertex(V3, Vec3f(0,0,-1), UV11)
	),
	Face( // EAST
		Vertex(V3, Vec3f(1,0,0), UV11),
		Vertex(V2, Vec3f(1,0,0), UV01),
		Vertex(V6, Vec3f(1,0,0), UV00)
	),
	Face( // WEST
		Vertex(V5, Vec3f(-1,0,0), UV01),
		Vertex(V1, Vec3f(-1,0,0), UV00),
		Vertex(V4, Vec3f(-1,0,0), UV10)
	),
};

constexpr const Face	FACE2[6] =
{
	Face( // TOP
		Vertex(V2, Vec3f(0,1,0), UV11),
		Vertex(V3, Vec3f(0,1,0), UV10),
		Vertex(V4, Vec3f(0,1,0), UV00)
	),
	Face( // BOTTOM
		Vertex(V8, Vec3f(0,-1,0), UV01),
		Vertex(V7, Vec3f(0,-1,0), UV11),
		Vertex(V6, Vec3f(0,-1,0), UV10)
	),
	Face( // NORTH
		Vertex(V5, Vec3f(0,0,1), UV00),
		Vertex(V6, Vec3f(0,0,1), UV10),
		Vertex(V2, Vec3f(0,0,1), UV11)
	),
	Face( // SOUTH
		Vertex(V8, Vec3f(0,0,-1), UV00),
		Vertex(V3, Vec3f(0,0,-1), UV11),
		Vertex(V7, Vec3f(0,0,-1), UV10)
	),
	Face( // EAST
		Vertex(V3, Vec3f(1,0,0), UV11),
		Vertex(V6, Vec3f(1,0,0), UV00),
		Vertex(V7, Vec3f(1,0,0), UV10)
	),
	Face( // WEST
		Vertex(V5, Vec3f(-1,0,0), UV01),
		Vertex(V4, Vec3f(-1,0,0), UV10),
		Vertex(V8, Vec3f(-1,0,0), UV11)
	),
};

void	Chunk::mesh()
{
	std::unique_lock<std::mutex>	lock(_chunkMutex);

	_mesh.clear();

	std::shared_ptr<Chunk>	neighbours[6] = {0};

	for (int dir = 0; dir < 6; dir++)
	{
		std::shared_ptr<Chunk> chunk = _world->getChunk(_pos + DIR_OFFSET[dir]);
		if (chunk && chunk->getState() >= Chunk::State::GENERATED)
			neighbours[dir] = chunk;
	}

	ChunkLocalVec3i	pos;

	ChunkLocalVec3i	blockPos;
	for (blockPos.x = 0; blockPos.x < CHUNK_SIZE; blockPos.x++)
		for (blockPos.y = 0; blockPos.y < CHUNK_SIZE; blockPos.y++)
			for (blockPos.z = 0; blockPos.z < CHUNK_SIZE; blockPos.z++)
			{
				if (!_isInBounds(blockPos))
					continue ;

				BlockStateId	block = getBlock(blockPos);

				if (block != 0)
				{
					for (int dir = 0; dir < 6; dir++)
					{
						block = 0;

						ChunkLocalVec3i	thisChunkPos = blockPos + DIR_OFFSET[dir];
						ChunkLocalVec3i	neighbourChunkPos = blockPos + DIR_OFFSET[dir] - (CHUNK_SIZE * DIR_OFFSET[dir]);

						if (_isInBounds(thisChunkPos))
							block = getBlock(thisChunkPos);
						else if (neighbours[dir] && neighbours[dir]->_isInBounds(neighbourChunkPos))
							block = neighbours[dir]->getBlock(neighbourChunkPos);

						if (block == 0)
						{
							Face	f1 = FACE1[dir];
							Face	f2 = FACE2[dir];
							f1.v1.pos = blockPos + f1.v1.pos;
							f1.v2.pos = blockPos + f1.v2.pos;
							f1.v3.pos = blockPos + f1.v3.pos;
							f2.v1.pos = blockPos + f2.v1.pos;
							f2.v2.pos = blockPos + f2.v2.pos;
							f2.v3.pos = blockPos + f2.v3.pos;

							Vec3f	random_color = {(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX};

							f2.v1.color = Vec3f(random_color);
							f2.v2.color = Vec3f(random_color);
							f2.v3.color = Vec3f(random_color);
							f1.v1.color = Vec3f(random_color);
							f1.v2.color = Vec3f(random_color);
							f1.v3.color = Vec3f(random_color);
							_mesh.push_back(f1.v1);
							_mesh.push_back(f1.v2);
							_mesh.push_back(f1.v3);
							_mesh.push_back(f2.v1);
							_mesh.push_back(f2.v2);
							_mesh.push_back(f2.v3);
						}
					}
				}
			}
	_state = Chunk::State::MESHED;
}

void	Chunk::draw(Shader &shader)
{
	shader.use();

	shader.setMat4f("model", translate<float>(_pos * CHUNK_SIZE));
	shader.setFloat("spawn_fade", _spawn_fade / SPAWN_FADE_TIME);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, _mesh_size);
	glBindVertexArray(0);
}

void	Chunk::save(const std::string &path)
{
	std::ofstream	file;

	file.open(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("chunk failed to open " + path);

	Chunk::Header	hdr;

	hdr.size = CHUNK_SIZE;
	std::memcpy(hdr.magic, CHUNK_HEADER_MAGIC, CHUNK_HEADER_MAGIC_SIZE);

	file.write(reinterpret_cast<char*>(&hdr), sizeof(Chunk::Header));
	file.write(reinterpret_cast<char*>(_blocks.data()), _blocks.size() * sizeof(ChunkBlockStateId));
}

void	Chunk::load(const std::string &path)
{
	struct stat	s;
	stat(path.c_str(), &s);
	if (s.st_size != sizeof(Chunk::Header) + CHUNK_VOLUME * sizeof(ChunkBlockStateId))
		throw std::runtime_error("Invalid save file size");

	std::ifstream	file;

	file.open(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("chunk failed to open " + path);

	Chunk::Header	hdr;

	file.read(reinterpret_cast<char*>(&hdr), sizeof(Chunk::Header));

	if (memcmp(hdr.magic, CHUNK_HEADER_MAGIC, CHUNK_HEADER_MAGIC_SIZE))
		throw std::runtime_error("Invalid chunk header magic");
	if (hdr.size != CHUNK_SIZE)
		throw std::runtime_error("Invalid chunk size");

	_blocks.resize(CHUNK_VOLUME);

	file.read(reinterpret_cast<char*>(_blocks.data()), CHUNK_VOLUME * sizeof(ChunkBlockStateId));
}

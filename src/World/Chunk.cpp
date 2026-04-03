#include "Chunk.hpp"
#include "World.hpp"
#include "Noise.hpp"
#include "Biome.hpp"

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

float smoothstep(float edge0, float edge1, float x)
{
	float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return (t * t * (3.0 - 2.0 * t));
}

std::vector<std::shared_ptr<Biome>>	ALL_BIOMES;

void	Chunk::generate(/*Generator *gen*/)
{
	if (try_load(get_chunk_path(_pos)))
		_edited = true;
	else
	{
		_blocks.resize(CHUNK_VOLUME);

		ChunkLocalVec3i	pos;

		std::vector<int>	height_map;
		std::vector<std::shared_ptr<Biome>>	biome_map;

		height_map.resize(CHUNK_SIZE * CHUNK_SIZE);
		biome_map.resize(CHUNK_SIZE * CHUNK_SIZE);

		for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
			for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
			{
				WorldVec3i	wp = chunkLocalToWorld(pos, _pos, CHUNK_SIZE);

				std::shared_ptr<Biome>	dominant_biome;
				float					height;

				Biome::get_biome(Vec2i(wp.x, wp.z), dominant_biome, height);

				height_map[pos.x + pos.z * CHUNK_SIZE] = height;
				biome_map[pos.x + pos.z * CHUNK_SIZE] = dominant_biome;
			}

		for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
			for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
				for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y++)
				{
					WorldVec3i	wp = chunkLocalToWorld(pos, _pos, CHUNK_SIZE);

					int	y = height_map[pos.x + pos.z * CHUNK_SIZE];

					if (!biome_map[pos.x + pos.z * CHUNK_SIZE])
						_setBlock(pos, BLOCK_BEDROCK);
					else
						_setBlock(pos, biome_map[pos.x + pos.z * CHUNK_SIZE]->get_block(wp, y));
				}
	}

	_state = Chunk::State::GENERATED;

	mesh_neighbours();

	mesh();
}

#include <glad/glad.h>

void	Chunk::upload()
{
	if (!_chunkMutex.try_lock())
		return ;

	_solid_mesh.upload();
	_transparent_mesh.upload();

	_state = Chunk::State::UPLOADED;

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

Face	FACE1[6] =
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

Face	FACE2[6] =
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

constexpr const Face GRASS_FACES[8] =
{
	// (\) front
	Face(
		Vertex(Vec3f(0, 0, 0), Vec3f(0,1,0), UV00),
		Vertex(Vec3f(1, 0, 1), Vec3f(0,1,0), UV10),
		Vertex(Vec3f(1, 1, 1), Vec3f(0,1,0), UV11)
	),
	Face(
		Vertex(Vec3f(0, 0, 0), Vec3f(0,1,0), UV00),
		Vertex(Vec3f(1, 1, 1), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(0, 1, 0), Vec3f(0,1,0), UV01)
	),

	// (\) back
	Face(
		Vertex(Vec3f(1, 1, 1), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(1, 0, 1), Vec3f(0,1,0), UV10),
		Vertex(Vec3f(0, 0, 0), Vec3f(0,1,0), UV00)
	),
	Face(
		Vertex(Vec3f(0, 1, 0), Vec3f(0,1,0), UV01),
		Vertex(Vec3f(1, 1, 1), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(0, 0, 0), Vec3f(0,1,0), UV00)
	),

	// (/) front
	Face(
		Vertex(Vec3f(0, 0, 1), Vec3f(0,1,0), UV00),
		Vertex(Vec3f(1, 0, 0), Vec3f(0,1,0), UV10),
		Vertex(Vec3f(1, 1, 0), Vec3f(0,1,0), UV11)
	),
	Face(
		Vertex(Vec3f(0, 0, 1), Vec3f(0,1,0), UV00),
		Vertex(Vec3f(1, 1, 0), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(0, 1, 1), Vec3f(0,1,0), UV01)
	),

	// (/) back
	Face(
		Vertex(Vec3f(1, 1, 0), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(1, 0, 0), Vec3f(0,1,0), UV10),
		Vertex(Vec3f(0, 0, 1), Vec3f(0,1,0), UV00)
	),
	Face(
		Vertex(Vec3f(0, 1, 1), Vec3f(0,1,0), UV01),
		Vertex(Vec3f(1, 1, 0), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(0, 0, 1), Vec3f(0,1,0), UV00)
	),
};

#define TEX_SIZE 16
#define ATLAS_SIZE 256
static Vec2f	getAtlasUV(Vec2f uv, int textureId)
{
	Vec2f	baseUV = uv;

	int row = (TEX_SIZE - 1) - (textureId / TEX_SIZE);;
	int col = textureId % TEX_SIZE;

	Vec2f cellSize = Vec2f((float)TEX_SIZE / (float)ATLAS_SIZE);
	Vec2f atlasOffset;
	atlasOffset.x = (float)col * cellSize.x;
	atlasOffset.y = (float)row * cellSize.y;
	Vec2f atlasUV;
	atlasUV.x = atlasOffset.x + baseUV.x * cellSize.x;
	atlasUV.y = atlasOffset.y + baseUV.y * cellSize.y;

	return (atlasUV);
}

void	Chunk::mesh_neighbours()
{
	for (int dir = 0; dir < 6; dir++)
	{
		std::shared_ptr<Chunk> chunk = _world->getChunk(_pos + DIR_OFFSET[dir]);
		if (chunk && chunk->state() >= Chunk::State::MESHED)
			chunk->mesh();
	}
}

void	Chunk::mesh()
{
	std::unique_lock<std::mutex>	lock(_chunkMutex);

	_solid_mesh.clear_data();
	_transparent_mesh.clear_data();

	std::shared_ptr<Chunk>	neighbours[6] = {0};

	for (int dir = 0; dir < 6; dir++)
	{
		std::shared_ptr<Chunk> chunk = _world->getChunk(_pos + DIR_OFFSET[dir]);
		if (chunk && chunk->state() >= Chunk::State::GENERATED)
			neighbours[dir] = chunk;
	}

	ChunkLocalVec3i	blockPos;
	for (blockPos.x = 0; blockPos.x < CHUNK_SIZE; blockPos.x++)
		for (blockPos.y = 0; blockPos.y < CHUNK_SIZE; blockPos.y++)
			for (blockPos.z = 0; blockPos.z < CHUNK_SIZE; blockPos.z++)
			{
				if (!_isInBounds(blockPos))
					continue ;

				BlockStateId	block = getBlock(blockPos);

				if (block != BLOCK_AIR)
				{
					if (block == BLOCK_TALL_GRASS || block == BLOCK_DEAD_BUSH || block == BLOCK_ROSE || block == BLOCK_DANDELION)
					{
						for (auto face : GRASS_FACES)
						{
							face.v1.pos = blockPos + face.v1.pos;
							face.v2.pos = blockPos + face.v2.pos;
							face.v3.pos = blockPos + face.v3.pos;

							face.v1.uv = getAtlasUV(face.v1.uv, block);
							face.v2.uv = getAtlasUV(face.v2.uv, block);
							face.v3.uv = getAtlasUV(face.v3.uv, block);

							Vec3f	color(1);

							if (block == BLOCK_TALL_GRASS) // Is grass / leaves and all
								color = Vec3f(0.05, 0.55, 0.05);

							face.v1.color = color;
							face.v2.color = color;
							face.v3.color = color;

							_solid_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&face), sizeof(face));
						}
						continue ;
					}

					for (int dir = 0; dir < 6; dir++)
					{
						BlockStateId	cull_block = 0;

						ChunkLocalVec3i	thisChunkPos = blockPos + DIR_OFFSET[dir];
						ChunkLocalVec3i	neighbourChunkPos = blockPos + DIR_OFFSET[dir] - (CHUNK_SIZE * DIR_OFFSET[dir]);

						if (!_isInBounds(thisChunkPos) && !neighbours[dir])
							continue ;

						if (_isInBounds(thisChunkPos))
							cull_block = getBlock(thisChunkPos);
						else if (neighbours[dir]->_isInBounds(neighbourChunkPos))
							cull_block = neighbours[dir]->getBlock(neighbourChunkPos);

						if ((cull_block == BLOCK_AIR || cull_block == BLOCK_WATER || cull_block == BLOCK_ICE || cull_block == BLOCK_TALL_GRASS || cull_block == BLOCK_DEAD_BUSH || cull_block == BLOCK_ROSE || cull_block == BLOCK_DANDELION) && cull_block != block)
						{
							Face	f1 = FACE1[dir];
							Face	f2 = FACE2[dir];
							f1.v1.pos = blockPos + f1.v1.pos;
							f1.v2.pos = blockPos + f1.v2.pos;
							f1.v3.pos = blockPos + f1.v3.pos;
							f2.v1.pos = blockPos + f2.v1.pos;
							f2.v2.pos = blockPos + f2.v2.pos;
							f2.v3.pos = blockPos + f2.v3.pos;

							int		atlasId = block;

							Vec3f	random_color = 1;
							if (block == BLOCK_GRASS) // Is grass / leaves and all
								random_color = Vec3f(0.05, 0.55, 0.05);

							f2.v1.color = Vec3f(random_color);
							f2.v2.color = Vec3f(random_color);
							f2.v3.color = Vec3f(random_color);
							f1.v1.color = Vec3f(random_color);
							f1.v2.color = Vec3f(random_color);
							f1.v3.color = Vec3f(random_color);

							f1.v1.uv = getAtlasUV(f1.v1.uv, atlasId);
							f1.v2.uv = getAtlasUV(f1.v2.uv, atlasId);
							f1.v3.uv = getAtlasUV(f1.v3.uv, atlasId);
							f2.v1.uv = getAtlasUV(f2.v1.uv, atlasId);
							f2.v2.uv = getAtlasUV(f2.v2.uv, atlasId);
							f2.v3.uv = getAtlasUV(f2.v3.uv, atlasId);
							if (block == BLOCK_WATER || block == BLOCK_ICE)
							{
								_transparent_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&f1), sizeof(f1));
								_transparent_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&f2), sizeof(f2));
							}
							else
							{
								_solid_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&f1), sizeof(f1));
								_solid_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&f2), sizeof(f2));
							}
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
	shader.setFloat("spawn_fade", SPAWN_FADE_TIME == 0 ? 1 : _spawn_fade / SPAWN_FADE_TIME);

	_solid_mesh.draw();
	_transparent_mesh.draw();
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
	std::ifstream	file;

	file.open(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("chunk failed to open " + path);

	struct stat	s;
	stat(path.c_str(), &s);
	if (s.st_size != sizeof(Chunk::Header) + CHUNK_VOLUME * sizeof(ChunkBlockStateId))
		throw std::runtime_error("Invalid save file size");

	Chunk::Header	hdr;

	file.read(reinterpret_cast<char*>(&hdr), sizeof(Chunk::Header));

	if (memcmp(hdr.magic, CHUNK_HEADER_MAGIC, CHUNK_HEADER_MAGIC_SIZE))
		throw std::runtime_error("Invalid chunk header magic");
	if (hdr.size != CHUNK_SIZE)
		throw std::runtime_error("Invalid chunk size");

	_blocks.resize(CHUNK_VOLUME);

	file.read(reinterpret_cast<char*>(_blocks.data()), CHUNK_VOLUME * sizeof(ChunkBlockStateId));

	for (auto b : _blocks)
		if (b != BLOCK_AIR)
			_non_air_blocks++;
}

bool	Chunk::try_load(const std::string &path)
{
	try
	{
		load(path);
		return (true);
	}
	catch (const std::exception &e)
	{
		return (false);
	}
	return (false);
}

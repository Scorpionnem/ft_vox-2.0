#include "Chunk.hpp"
#include "World.hpp"
#include "Noise.hpp"

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

#define BLOCK_COBBLESTONE 16
#define BLOCK_SANDSTONE 208
#define BLOCK_BEDROCK 17
#define BLOCK_GRAVEL 19
#define BLOCK_GRASS 40
#define BLOCK_STONE 1
#define BLOCK_SAND 18
#define BLOCK_DIRT 2
#define BLOCK_AIR 0
#define BLOCK_WATER 223

float smoothstep(float edge0, float edge1, float x)
{
	float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return (t * t * (3.0 - 2.0 * t));
}

struct	Range
{
	Range(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

	float	min;
	float	max;
};

struct	Biome
{
	Range	continentalness;
	Range	erosion;
	Range	riverness;

	Range	temperature;
	Range	humidity;

	float	scale;
	int		height;
	int		min_height;
	int		noisiness;

	int		top_block;
};

Biome	BIOME_DEEP_OCEAN = {
	.continentalness = {-1.0, -0.4},
	.erosion = {0.1, 1},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.025,
	.height = 3,
	.min_height = 0,
	.noisiness = 1,

	.top_block = BLOCK_STONE
};

Biome	BIOME_OCEAN = {
	.continentalness = {-0.4, 0.0},
	.erosion = {-1, 1},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.06,
	.height = 8,
	.min_height = 30,
	.noisiness = 1,

	.top_block = BLOCK_GRAVEL
};

Biome	BIOME_SHALLOW_OCEAN = {
	.continentalness = {-0.4, 0.15},
	.erosion = {0.3, 1},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.015,
	.height = 5,
	.min_height = 54,
	.noisiness = 1,

	.top_block = BLOCK_SANDSTONE,
};

Biome	BIOME_BEACHES = {
	.continentalness = {0, 0.1},
	.erosion = {-0.2, 0.2},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.015,
	.height = 16,
	.min_height = 70,
	.noisiness = 1,

	.top_block = BLOCK_SAND,
};

Biome	BIOME_PLAINS = {
	.continentalness = {0.2, 0.7},
	.erosion = {0.0, 1},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.015,
	.height = 32,
	.min_height = 85,
	.noisiness = 1,

	.top_block = BLOCK_GRASS,
};

Biome	BIOME_HIGHLANDS = {
	.continentalness = {0.4, 0.7},
	.erosion = {-1, 0.4},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.025,
	.height = 48,
	.min_height = 100,
	.noisiness = 1,

	.top_block = BLOCK_GRASS,
};

Biome	BIOME_MOUNTAINS = {
	.continentalness = {0.37, 1},
	.erosion = {-1, -0.0},
	.riverness = {0, 0},

	.temperature = {0, 0},
	.humidity = {0, 0},

	.scale = 0.010,
	.height = 128,
	.min_height = 120,
	.noisiness = 5,

	.top_block = BLOCK_STONE
};

std::vector<Biome>	ALL_BIOMES = {
	BIOME_DEEP_OCEAN,
	BIOME_OCEAN,
	BIOME_SHALLOW_OCEAN,
	BIOME_BEACHES,
	BIOME_PLAINS,
	BIOME_HIGHLANDS,
	BIOME_MOUNTAINS,
};

float distance_to_range(float v, Range r)
{
	if (v < r.min)
		return (r.min - v);
	if (v > r.max)
		return (v - r.max);
	return (0.0f);
}

void	Chunk::generate(/*Generator *gen*/)
{
	_blocks.resize(CHUNK_VOLUME);

	ChunkLocalVec3i	pos;

	std::vector<int>	height_map;
	std::vector<Biome*>	biome_map;

	height_map.resize(CHUNK_SIZE * CHUNK_SIZE);
	biome_map.resize(CHUNK_SIZE * CHUNK_SIZE);

	for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
		for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
		{
			WorldVec3i	wp = pos + (_pos * CHUNK_SIZE);

			float	continentalness = noise(Vec2f(wp.x, wp.z), 0.0025, 1, 5);
			float	erosion = noise(Vec2f(wp.x, wp.z), 0.0075, 1, 3);

			std::vector<std::pair<float, Biome*>>	biome_weights;
			for (auto &biome : ALL_BIOMES)
			{
				float	weight = 0;

				weight += distance_to_range(continentalness, biome.continentalness);
				weight += distance_to_range(erosion, biome.erosion);

				biome_weights.push_back(std::make_pair(weight, &biome));
			}

			float sharpness = 32.0f;
			for (auto &[w, b] : biome_weights)
				w = exp(-w * sharpness);

			float total = 0.0f;
			for (auto [w, b] : biome_weights)
				total += w;
			for (auto &[w, b] : biome_weights)
				w /= total;

			float	biggest_weight = 0;
			biome_map[pos.x + pos.z * CHUNK_SIZE] = &BIOME_PLAINS;

			float height = 0.0f;
			for (uint64_t i = 0; i < biome_weights.size(); i++)
			{
				if (biome_weights[i].first > 0.01)
				{
					Biome	*biome = biome_weights[i].second;
					float	biome_height = noise(Vec2f(wp.x, wp.z), biome->scale, 1, biome->noisiness) * biome->height + biome->min_height;

					height += biome_height * biome_weights[i].first;
				}

				if (biome_weights[i].first > biggest_weight)
				{
					biggest_weight = biome_weights[i].first;
					biome_map[pos.x + pos.z * CHUNK_SIZE] = biome_weights[i].second;
				}
			}

			height_map[pos.x + pos.z * CHUNK_SIZE] = height;
		}

	for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
		for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
			for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y++)
			{
				WorldVec3i	wp = pos + (_pos * CHUNK_SIZE);

				int	y = height_map[pos.x + pos.z * CHUNK_SIZE];

				if (wp.y <= y)
				{
					if (wp.y == y) // Surface block
						setBlock(pos, biome_map[pos.x + pos.z * CHUNK_SIZE]->top_block);
					else if (wp.y == y - 1) // Sub-Surface block
						setBlock(pos, BLOCK_DIRT);
					else // Underground block
						setBlock(pos, BLOCK_STONE);
				}
				else if (wp.y < 60)
					setBlock(pos, BLOCK_WATER);
				else
					setBlock(pos, BLOCK_AIR);
			}
	_state = Chunk::State::GENERATED;
	for (int dir = 0; dir < 6; dir++)
	{
		std::shared_ptr<Chunk> chunk = _world->getChunk(_pos + DIR_OFFSET[dir]);
		if (chunk && chunk->state() >= Chunk::State::MESHED)
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

	_mesh.insert(_mesh.end(), _transparent_mesh.begin(), _transparent_mesh.end());

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
	_transparent_mesh.clear();

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

void	Chunk::mesh()
{
	std::unique_lock<std::mutex>	lock(_chunkMutex);

	_mesh.clear();

	std::shared_ptr<Chunk>	neighbours[6] = {0};

	for (int dir = 0; dir < 6; dir++)
	{
		std::shared_ptr<Chunk> chunk = _world->getChunk(_pos + DIR_OFFSET[dir]);
		if (chunk && chunk->state() >= Chunk::State::GENERATED)
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
						BlockStateId	cull_block = 0;

						ChunkLocalVec3i	thisChunkPos = blockPos + DIR_OFFSET[dir];
						ChunkLocalVec3i	neighbourChunkPos = blockPos + DIR_OFFSET[dir] - (CHUNK_SIZE * DIR_OFFSET[dir]);

						if (!_isInBounds(thisChunkPos) && !neighbours[dir])
							continue ;

						if (_isInBounds(thisChunkPos))
							cull_block = getBlock(thisChunkPos);
						else if (neighbours[dir]->_isInBounds(neighbourChunkPos))
							cull_block = neighbours[dir]->getBlock(neighbourChunkPos);

						if ((cull_block == 0 || cull_block == BLOCK_WATER) && cull_block != block)
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
							if (block == 40) // Is grass / leaves and all
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
							if (block == BLOCK_WATER)
							{
								_transparent_mesh.push_back(f1.v1);
								_transparent_mesh.push_back(f1.v2);
								_transparent_mesh.push_back(f1.v3);
								_transparent_mesh.push_back(f2.v1);
								_transparent_mesh.push_back(f2.v2);
								_transparent_mesh.push_back(f2.v3);
							}
							else
							{
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
			}
	_state = Chunk::State::MESHED;
}

void	Chunk::draw(Shader &shader)
{
	shader.use();

	shader.setMat4f("model", translate<float>(_pos * CHUNK_SIZE));
	shader.setFloat("spawn_fade", SPAWN_FADE_TIME == 0 ? 1 : _spawn_fade / SPAWN_FADE_TIME);

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

bool	Chunk::try_load(const std::string &path)
{
	try
	{
		load(path);
		return (true);
	}
	catch (...)
	{
		return (false);
	}
	return (false);
}

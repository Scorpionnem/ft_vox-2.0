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
		Vertex(V5, Vec3f(-1,0,0), UV00),
		Vertex(V1, Vec3f(-1,0,0), UV01),
		Vertex(V4, Vec3f(-1,0,0), UV11)
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
		Vertex(V5, Vec3f(-1,0,0), UV00),
		Vertex(V4, Vec3f(-1,0,0), UV11),
		Vertex(V8, Vec3f(-1,0,0), UV10)
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

constexpr const Face CACTUS_FACES[12] =
{
	Face( // NORTH
		Vertex(Vec3f(0, 0, 0.9375), Vec3f(0,0,1), UV00),
		Vertex(Vec3f(1, 0, 0.9375), Vec3f(0,0,1), UV10),
		Vertex(Vec3f(1, 1, 0.9375), Vec3f(0,0,1), UV11)
	),
	Face( // NORTH
		Vertex(Vec3f(0, 0, 0.9375), Vec3f(0,0,1), UV00),
		Vertex(Vec3f(1, 1, 0.9375), Vec3f(0,0,1), UV11),
		Vertex(Vec3f(0, 1, 0.9375), Vec3f(0,0,1), UV01)
	),

	Face( // EAST
		Vertex(Vec3f(0.9375, 1, 1), Vec3f(1,0,0), UV11),
		Vertex(Vec3f(0.9375, 0, 1), Vec3f(1,0,0), UV10),
		Vertex(Vec3f(0.9375, 0, 0), Vec3f(1,0,0), UV00)
	),
	Face( // EAST
		Vertex(Vec3f(0.9375, 1, 0), Vec3f(1,0,0), UV01),
		Vertex(Vec3f(0.9375, 1, 1), Vec3f(1,0,0), UV11),
		Vertex(Vec3f(0.9375, 0, 0), Vec3f(1,0,0), UV00)
	),

	Face( // WEST
		Vertex(Vec3f(0.0625, 1, 0), Vec3f(-1,0,0), UV11),
		Vertex(Vec3f(0.0625, 0, 0), Vec3f(-1,0,0), UV10),
		Vertex(Vec3f(0.0625, 0, 1), Vec3f(-1,0,0), UV00)
	),
	Face( // WEST
		Vertex(Vec3f(0.0625, 1, 1), Vec3f(-1,0,0), UV01),
		Vertex(Vec3f(0.0625, 1, 0), Vec3f(-1,0,0), UV11),
		Vertex(Vec3f(0.0625, 0, 1), Vec3f(-1,0,0), UV00)
	),

	Face( // SOUTH
		Vertex(Vec3f(1, 1, 0.0625), Vec3f(0,0,-1), UV11),
		Vertex(Vec3f(1, 0, 0.0625), Vec3f(0,0,-1), UV10),
		Vertex(Vec3f(0, 0, 0.0625), Vec3f(0,0,-1), UV00)
	),
	Face( // SOUTH
		Vertex(Vec3f(0, 1, 0.0625), Vec3f(0,0,-1), UV01),
		Vertex(Vec3f(1, 1, 0.0625), Vec3f(0,0,-1), UV11),
		Vertex(Vec3f(0, 0, 0.0625), Vec3f(0,0,-1), UV00)
	),

	Face( // TOP
		Vertex(V2, Vec3f(0,1,0), UV11),
		Vertex(V3, Vec3f(0,1,0), UV10),
		Vertex(V4, Vec3f(0,1,0), UV00)
	),
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
	Face( // BOTTOM
		Vertex(V8, Vec3f(0,-1,0), UV01),
		Vertex(V7, Vec3f(0,-1,0), UV11),
		Vertex(V6, Vec3f(0,-1,0), UV10)
	),
};

constexpr const Face LILYPAD_FACES[4] =
{
	Face( // TOP
		Vertex(Vec3f(1, 0.0625, 1), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(1, 0.0625, 0), Vec3f(0,1,0), UV10),
		Vertex(Vec3f(0, 0.0625, 0), Vec3f(0,1,0), UV00)
	),
	Face( // TOP
		Vertex(Vec3f(0, 0.0625, 1), Vec3f(0,1,0), UV01),
		Vertex(Vec3f(1, 0.0625, 1), Vec3f(0,1,0), UV11),
		Vertex(Vec3f(0, 0.0625, 0), Vec3f(0,1,0), UV00)
	),

	Face( // BOTTOM
		Vertex(Vec3f(0, 0.0625, 0), Vec3f(0,-1,0), UV01),
		Vertex(Vec3f(1, 0.0625, 1), Vec3f(0,-1,0), UV10),
		Vertex(Vec3f(0, 0.0625, 1), Vec3f(0,-1,0), UV00)
	),
	Face( // BOTTOM
		Vertex(Vec3f(0, 0.0625, 0), Vec3f(0,-1,0), UV01),
		Vertex(Vec3f(1, 0.0625, 0), Vec3f(0,-1,0), UV11),
		Vertex(Vec3f(1, 0.0625, 1), Vec3f(0,-1,0), UV10)
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

std::vector<std::shared_ptr<Biome>>	ALL_BIOMES;

void	Chunk::_generateFeatures()
{
	ChunkWorldVec3i	chunk_pos;
	for (chunk_pos.x = _pos.x - 1; chunk_pos.x <= _pos.x + 1; chunk_pos.x++)
		for (chunk_pos.z = _pos.z - 1; chunk_pos.z <= _pos.z + 1; chunk_pos.z++)
			for (chunk_pos.y = _pos.y - 1; chunk_pos.y <= _pos.y + 1; chunk_pos.y++)
			{
				#define STRUCTURES_PER_CHUNK 8
				for (int i = 0; i < STRUCTURES_PER_CHUNK; i++)
				{
					ChunkWorldVec3i	gen_chunk_pos = Vec3i(chunk_pos.x, 0, chunk_pos.z);
					ChunkWorldVec3i	spiced_gen_chunk_pos = Vec3i(chunk_pos.x + i, 0, chunk_pos.z + i);
					WorldVec3i	structure_pos = chunkLocalToWorld(rand3dTo3d(spiced_gen_chunk_pos) * CHUNK_SIZE, gen_chunk_pos, CHUNK_SIZE);

					std::shared_ptr<Biome>	dominant_biome;
					float					max_height;
					Biome::get_biome(Vec2i(structure_pos.x, structure_pos.z), dominant_biome, max_height);

					structure_pos.y = max_height;

					Structure	*feat = dominant_biome->get_surface_feature(structure_pos);
					if (!feat)
						continue ;

					auto blocks = feat->generate(structure_pos);
					for (auto &b : blocks)
						_setBlockFromWorld(b.pos, b.block);
				}
			}
}

bool	is_inside(Vec3f pos_a, Vec3f size_a, Vec3f pos_b, Vec3f size_b);

void	Chunk::_generateStructures()
{
	RegionWorldVec3i	cur_region_pos = worldToRegionWorld(_pos * CHUNK_SIZE, REGION_SIZE);
	RegionWorldVec3i	region_pos;
	for (region_pos.x = cur_region_pos.x - 1; region_pos.x <= cur_region_pos.x + 1; region_pos.x++)
		for (region_pos.z = cur_region_pos.z - 1; region_pos.z <= cur_region_pos.z + 1; region_pos.z++)
			for (region_pos.y = cur_region_pos.y - 1; region_pos.y <= cur_region_pos.y + 1; region_pos.y++)
			{
				#define ANY_STRUCTURES_PER_REGION 8
				for (int i = 0; i < ANY_STRUCTURES_PER_REGION; i++)
				{
					RegionWorldVec3i	gen_region_pos = Vec3i(region_pos.x, region_pos.y, region_pos.z);
					RegionWorldVec3i	spiced_gen_region_pos = Vec3i(region_pos.x + i, region_pos.y + i, region_pos.z + i);
					WorldVec3i			structure_pos = regionLocalToWorld(rand3dTo3d(spiced_gen_region_pos) * REGION_SIZE, gen_region_pos, REGION_SIZE);

					std::shared_ptr<Biome>	dominant_biome;
					float					max_height;
					Biome::get_biome(Vec2i(structure_pos.x, structure_pos.z), dominant_biome, max_height);

					Structure	*feat = dominant_biome->get_structure(structure_pos);
					if (!feat || feat) // TODO REMOVE TO HAVE FLYING STRUCTURES AND SHI
						continue ;

					// maximum size of the structure
					int	STRUCTURE_SIZE = 64;
					if (!is_inside(_pos * CHUNK_SIZE, CHUNK_SIZE, structure_pos - STRUCTURE_SIZE, REGION_SIZE))
						continue ;

					auto blocks = feat->generate(structure_pos);
					for (auto &b : blocks)
						_setBlockFromWorld(b.pos, b.block);
				}
				#define SURFACE_STRUCTURES_PER_REGION 8
				for (int i = 0; i < ANY_STRUCTURES_PER_REGION; i++)
				{
					RegionWorldVec3i	gen_region_pos = Vec3i(region_pos.x, 0, region_pos.z);
					RegionWorldVec3i	spiced_gen_region_pos = Vec3i(region_pos.x + i, 0, region_pos.z + i);
					WorldVec3i			structure_pos = regionLocalToWorld(rand3dTo3d(spiced_gen_region_pos) * REGION_SIZE, gen_region_pos, REGION_SIZE);

					std::shared_ptr<Biome>	dominant_biome;
					float					max_height;
					Biome::get_biome(Vec2i(structure_pos.x, structure_pos.z), dominant_biome, max_height);

					structure_pos.y = max_height;

					Structure	*feat = dominant_biome->get_structure(structure_pos);
					if (!feat)
						continue ;

					// maximum size of the structure
					int	STRUCTURE_SIZE = 64;
					if (!is_inside(_pos * CHUNK_SIZE, CHUNK_SIZE, structure_pos - STRUCTURE_SIZE, REGION_SIZE))
						continue ;

					auto blocks = feat->generate(structure_pos);
					for (auto &b : blocks)
						_setBlockFromWorld(b.pos, b.block);
				}
			}
}

float smoothstep(float edge0, float edge1, float x)
{
	float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return (t * t * (3.0 - 2.0 * t));
}

#define CAVE_NOISE_STEP_SIZE 4
#define CAVE_NOISE_MAP_SIZE (CHUNK_SIZE / CAVE_NOISE_STEP_SIZE + 1)

static uint16_t	_getCaveMapIndex(const Vec3i &pos)
{
	return (pos.x + pos.y * CAVE_NOISE_MAP_SIZE + pos.z * CAVE_NOISE_MAP_SIZE * CAVE_NOISE_MAP_SIZE);
}

static float	_sampleCaveMap(const std::vector<float> &cave_map, const Vec3f &grid_pos)
{
	int	x0 = static_cast<int>(grid_pos.x);
	int	y0 = static_cast<int>(grid_pos.y);
	int	z0 = static_cast<int>(grid_pos.z);

	int	x1 = x0 + 1;
	int	y1 = y0 + 1;
	int	z1 = z0 + 1;

	float	tx = grid_pos.x - x0;
	float	ty = grid_pos.y - y0;
	float	tz = grid_pos.z - z0;

	float	c000 = cave_map[_getCaveMapIndex(Vec3i(x0,y0,z0))];
	float	c100 = cave_map[_getCaveMapIndex(Vec3i(x1,y0,z0))];
	float	c010 = cave_map[_getCaveMapIndex(Vec3i(x0,y1,z0))];
	float	c110 = cave_map[_getCaveMapIndex(Vec3i(x1,y1,z0))];
	float	c001 = cave_map[_getCaveMapIndex(Vec3i(x0,y0,z1))];
	float	c101 = cave_map[_getCaveMapIndex(Vec3i(x1,y0,z1))];
	float	c011 = cave_map[_getCaveMapIndex(Vec3i(x0,y1,z1))];
	float	c111 = cave_map[_getCaveMapIndex(Vec3i(x1,y1,z1))];

	float	c00 = lerp(c000, c100, tx);
	float	c10 = lerp(c010, c110, tx);
	float	c01 = lerp(c001, c101, tx);
	float	c11 = lerp(c011, c111, tx);

	float	c0 = lerp(c00, c10, ty);
	float	c1 = lerp(c01, c11, ty);

	return (lerp(c0, c1, tz));
}

void	Chunk::_generateTerrain()
{
	std::vector<float> cave_noise_map;

	cave_noise_map.reserve(CAVE_NOISE_MAP_SIZE * CAVE_NOISE_MAP_SIZE * CAVE_NOISE_MAP_SIZE);

	ChunkLocalVec3i	pos;

	for (pos.x = 0; pos.x < CAVE_NOISE_MAP_SIZE; pos.x++)
		for (pos.y = 0; pos.y < CAVE_NOISE_MAP_SIZE; pos.y++)
			for (pos.z = 0; pos.z < CAVE_NOISE_MAP_SIZE; pos.z++)
			{
				WorldVec3i	wp = chunkLocalToWorld(ChunkLocalVec3i(pos.x * CAVE_NOISE_STEP_SIZE, pos.y * CAVE_NOISE_STEP_SIZE, pos.z * CAVE_NOISE_STEP_SIZE), _pos, CHUNK_SIZE);

				float	intensity = smoothstep(0.0, 100, 100);
				float	v = noise(wp, 0.01, 1, 4) * intensity;
				cave_noise_map[_getCaveMapIndex(pos)] = v;
			}

	for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
		for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
		{
			WorldVec3i	wp = chunkLocalToWorld(pos, _pos, CHUNK_SIZE);

			std::shared_ptr<Biome>	dominant_biome;
			float					max_height;

			Biome::get_biome(Vec2i(wp.x, wp.z), dominant_biome, max_height);

			for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y++)
			{
				WorldVec3i	wp = chunkLocalToWorld(pos, _pos, CHUNK_SIZE);

				Vec3f	cave_grid_pos = Vec3f(pos) / CAVE_NOISE_STEP_SIZE;

				float caveNoise = _sampleCaveMap(cave_noise_map, cave_grid_pos);
				if (wp.y <= max_height && caveNoise > 0.2)
				{
					_setBlock(pos, BLOCK_AIR);
					continue ;
				}

				if (!dominant_biome)
					_setBlock(pos, BLOCK_BEDROCK);
				else
					_setBlock(pos, dominant_biome->get_block(wp, max_height));
			}
		}
}

void	Chunk::generate(/*Generator *gen*/)
{
	if (try_load(get_chunk_path(_pos)))
		_edited = true;
	else
	{
		_blocks.resize(CHUNK_VOLUME);

		_generateTerrain();
		_generateFeatures();
		_generateStructures();
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

				BlockStateId	block = _getBlock(blockPos);

				if (block != BLOCK_AIR)
				{
					if (block == BLOCK_TALL_GRASS || block == BLOCK_DEAD_BUSH || block == BLOCK_ROSE || block == BLOCK_DANDELION || block == BLOCK_SUGARCANE)
					{
						for (auto face : GRASS_FACES)
						{
							face.v1.pos = (Vec3f)blockPos + face.v1.pos;
							face.v2.pos = (Vec3f)blockPos + face.v2.pos;
							face.v3.pos = (Vec3f)blockPos + face.v3.pos;

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

					if (block == BLOCK_CACTUS)
					{
						int	i = 0;
						for (auto face : CACTUS_FACES)
						{
							i++;

							face.v1.pos = (Vec3f)blockPos + face.v1.pos;
							face.v2.pos = (Vec3f)blockPos + face.v2.pos;
							face.v3.pos = (Vec3f)blockPos + face.v3.pos;

							face.v1.uv = getAtlasUV(face.v1.uv, block - ((i == 9 || i == 10) ? 1 : 0) + ((i == 11 || i == 12) ? 1 : 0));
							face.v2.uv = getAtlasUV(face.v2.uv, block - ((i == 9 || i == 10) ? 1 : 0) + ((i == 11 || i == 12) ? 1 : 0));
							face.v3.uv = getAtlasUV(face.v3.uv, block - ((i == 9 || i == 10) ? 1 : 0) + ((i == 11 || i == 12) ? 1 : 0));

							Vec3f	color(1);

							face.v1.color = color;
							face.v2.color = color;
							face.v3.color = color;

							_solid_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&face), sizeof(face));
						}
						continue ;
					}

					if (block == BLOCK_LILYPAD)
					{
						for (auto face : LILYPAD_FACES)
						{
							face.v1.pos = (Vec3f)blockPos + face.v1.pos;
							face.v2.pos = (Vec3f)blockPos + face.v2.pos;
							face.v3.pos = (Vec3f)blockPos + face.v3.pos;

							face.v1.uv = getAtlasUV(face.v1.uv, block);
							face.v2.uv = getAtlasUV(face.v2.uv, block);
							face.v3.uv = getAtlasUV(face.v3.uv, block);

							Vec3f	color(0.05, 0.55, 0.05);

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
							cull_block = _getBlock(thisChunkPos);
						else if (neighbours[dir]->_isInBounds(neighbourChunkPos))
							cull_block = neighbours[dir]->_getBlock(neighbourChunkPos);

						if ((cull_block == BLOCK_AIR || cull_block == BLOCK_WATER || cull_block == BLOCK_ICE || cull_block == BLOCK_TALL_GRASS || cull_block == BLOCK_DEAD_BUSH || cull_block == BLOCK_ROSE || cull_block == BLOCK_DANDELION || cull_block == BLOCK_OAK_LEAVES || cull_block == BLOCK_CACTUS || cull_block == BLOCK_SUGARCANE || cull_block == BLOCK_LILYPAD) && (cull_block != block || block == BLOCK_OAK_LEAVES))
						{
							Face	f1 = FACE1[dir];
							Face	f2 = FACE2[dir];
							f1.v1.pos = (Vec3f)blockPos + f1.v1.pos;
							f1.v2.pos = (Vec3f)blockPos + f1.v2.pos;
							f1.v3.pos = (Vec3f)blockPos + f1.v3.pos;
							f2.v1.pos = (Vec3f)blockPos + f2.v1.pos;
							f2.v2.pos = (Vec3f)blockPos + f2.v2.pos;
							f2.v3.pos = (Vec3f)blockPos + f2.v3.pos;

							int		atlasId = block;

							Vec3f	random_color = 1;
							if (block == BLOCK_GRASS || block == BLOCK_OAK_LEAVES) // Is grass / leaves and all
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

void	Chunk::draw(Shader &shader, Vec3d cam_pos)
{
	if (_state < Chunk::State::UPLOADED)
		upload();

	shader.use();

	shader.setMat4f("model", translate<float>(Vec3d(_pos * CHUNK_SIZE) - cam_pos));
	shader.setFloat("spawn_fade", SPAWN_FADE_TIME == 0 ? 1 : _spawn_fade / SPAWN_FADE_TIME);

	_solid_mesh.draw();
	_transparent_mesh.draw();
}

void	Chunk::save(const std::string &path)
{
	std::ofstream	file;

	file.open(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("chunk saving: failed to open " + path);

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
		throw std::runtime_error("chunk loading: failed to open " + path);

	struct stat	s;
	stat(path.c_str(), &s);
	if (s.st_size != sizeof(Chunk::Header) + CHUNK_VOLUME * sizeof(ChunkBlockStateId))
		throw std::runtime_error("chunk loading: invalid save file size");

	Chunk::Header	hdr;

	file.read(reinterpret_cast<char*>(&hdr), sizeof(Chunk::Header));

	if (memcmp(hdr.magic, CHUNK_HEADER_MAGIC, CHUNK_HEADER_MAGIC_SIZE))
		throw std::runtime_error("chunk loading: invalid chunk header magic");
	if (hdr.size != CHUNK_SIZE)
		throw std::runtime_error("chunk loading: invalid chunk size");

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
	catch (const std::exception &e)
	{
		return (false);
	}
	return (false);
}

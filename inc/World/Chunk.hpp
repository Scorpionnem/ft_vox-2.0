#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <array>
#include <cstring>
#include <sys/stat.h>
#include <atomic>

#include "Math.hpp"
#include "Shader.hpp"

/* Position in chunk local space (0-CHUNK_SIZE) */
using ChunkLocalVec3i	= 	Vec3i;
/* Position in chunk world space */
using ChunkWorldVec3i	= 	Vec3i;
/* Position in world space */
using WorldVec3i		= 	Vec3i;

#define CHUNK_SIZE			32
#define CHUNK_VOLUME		CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

/* DONT forget to implement local block state table when implementing blockstates */
using ChunkBlockStateId = 	uint16_t;
using BlockStateId = 		uint32_t;

struct	Vertex
{
	Vertex(){}
	Vertex(Vec3f pos, Vec3f normal, Vec2f uv)
	{
		this->pos = pos;
		this->normal = normal;
		this->uv = uv;
	}
	Vec3f	pos;
	Vec3f	normal;
	Vec2f	uv;
};

struct	Face
{
	Face(){}
	Face(Vertex v1, Vertex v2, Vertex v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
	Vertex	v1;
	Vertex	v2;
	Vertex	v3;
};

class	Chunk
{
	std::vector<Face>	_mesh;
	public:
		enum class	State
		{
			NONE,
			GENERATED,
			MESHED,
			UPLOADED,
			EDITED,
		};
	private:
		/*
			Header for the chunk save files
		*/
		#define CHUNK_HEADER_MAGIC		"VCK"
		#define CHUNK_HEADER_MAGIC_SIZE	sizeof(CHUNK_HEADER_MAGIC) - 1
		struct	Header
		{
			char		magic[CHUNK_HEADER_MAGIC_SIZE];
			uint32_t	size;
			// Local block state table
		};
	public:
		Chunk(const ChunkWorldVec3i &pos)
		{
			_pos = pos;
		}
		~Chunk() {}

		void	update(double delta)
		{
			(void)delta;
		}
		void	generate(/*Generator *gen*/);
		void	mesh();
		void	draw(Shader &shader);
		void	upload();

		void	save(const std::string &path);
		void	load(const std::string &path);

		inline ChunkBlockStateId	getBlock(const ChunkLocalVec3i &pos)
		{
			if (!_isInBounds(pos))
				throw std::runtime_error("getblock out of bounds");
			return (_blocks[_getBlockIndex(pos)]);
		}
		inline void	setBlock(const ChunkLocalVec3i &pos, ChunkBlockStateId block)
		{
			if (!_isInBounds(pos))
				throw std::runtime_error("setblock out of bounds");
			_blocks[_getBlockIndex(pos)] = block;
		}

		Chunk::State	getState() {return (_state);}
		ChunkWorldVec3i	getPos() {return (_pos);}
	private:
		ChunkWorldVec3i					_pos;

		std::vector<ChunkBlockStateId>	_blocks;

		std::atomic<Chunk::State>		_state = Chunk::State::NONE;

		uint	VAO;
		uint	VBO;
	private:
		inline bool	_isInBounds(const ChunkLocalVec3i &pos)
		{
			return (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE);
		}
		inline uint16_t	_getBlockIndex(const ChunkLocalVec3i &pos)
		{
			return (pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE * CHUNK_SIZE);
		}
};

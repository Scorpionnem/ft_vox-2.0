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
#include <mutex>

#include "Math.hpp"
#include "Shader.hpp"
#include "BlockType.hpp"
#include "Face.hpp"

#define CHUNK_SIZE			32
#define CHUNK_VOLUME		CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

#define SPAWN_FADE_TIME		0.3

class	World;

class	Chunk
{
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
		Chunk(const ChunkWorldVec3i &pos, World *world)
		{
			_world = world;
			_pos = pos;
		}
		~Chunk()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}

		void	update(double delta)
		{
			_spawn_fade += delta;
			if (_spawn_fade >= SPAWN_FADE_TIME)
				_spawn_fade = SPAWN_FADE_TIME;

			// for each updatable blocks do stuff
		}

		void	generate(/*Generator *gen*/);
		void	mesh();
		void	draw(Shader &shader);
		void	upload();

		/* Saves chunk data in a binary file (Only blocks) */
		void	save(const std::string &path);
		void	load(const std::string &path);
		/* Tries to load a chunk, returns false instead of throwing */
		bool	try_load(const std::string &path);

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

			ChunkBlockStateId	&b = _blocks[_getBlockIndex(pos)];

			if (b == 0 && block != 0)
				_non_air_blocks++;
			else if (b != 0 && block == 0)
				_non_air_blocks--;
			b = block;
		}

		Chunk::State	state() {return (_state);}
		ChunkWorldVec3i	pos() {return (_pos);}
		bool			has_solid_blocks()
		{
			return (_non_air_blocks != 0);
		}

		bool	check = true; // TODO Remove ts
	private:
		std::mutex						_chunkMutex;
		std::atomic<Chunk::State>		_state = Chunk::State::NONE;

		ChunkWorldVec3i					_pos;
		std::vector<ChunkBlockStateId>	_blocks;

		// need to abstract this
		uint32_t						VAO = 0;
		uint32_t						VBO = 0;
		uint64_t						_mesh_size = 0;
		uint16_t						_non_air_blocks = 0;
		std::vector<Vertex>				_mesh;
		std::vector<Vertex>				_transparent_mesh;

		World							*_world;

		float							_spawn_fade = 0;
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

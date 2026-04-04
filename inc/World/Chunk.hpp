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
#include <iostream>

#include "Math.hpp"
#include "Shader.hpp"
#include "BlockType.hpp"
#include "Face.hpp"
#include "Mesh.hpp"

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
			_solid_mesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
			_solid_mesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
			_solid_mesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(Vertex, color));
			_solid_mesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(Vertex, uv));
			_solid_mesh.set_sizeof_layout(sizeof(Vertex));

			_transparent_mesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
			_transparent_mesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
			_transparent_mesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(Vertex, color));
			_transparent_mesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(Vertex, uv));
			_transparent_mesh.set_sizeof_layout(sizeof(Vertex));

			_world = world;
			_pos = pos;
		}
		~Chunk()
		{
			if (_edited)
				save(get_chunk_path(_pos));
		}

		static std::string	get_chunk_path(const ChunkWorldVec3i &pos)
		{
			return ("saves/" + std::to_string(pos.hash()) + ".vck");
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
		void	mesh_neighbours();
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

			set_edited();

			_setBlock(pos, block);
		}

		Chunk::State	state() {return (_state);}
		ChunkWorldVec3i	pos() {return (_pos);}
		bool			has_solid_blocks()
		{
			return (_non_air_blocks != 0);
		}

		void	set_edited()
		{
			_edited = true;
		}
		bool	check = true; // TODO Remove ts
	private:
		std::mutex						_chunkMutex;
		std::atomic<Chunk::State>		_state = Chunk::State::NONE;
		std::atomic_bool				_edited = false;

		ChunkWorldVec3i					_pos;
		std::vector<ChunkBlockStateId>	_blocks;

		// need to abstract this
		uint16_t						_non_air_blocks = 0;

		Mesh							_solid_mesh;
		Mesh							_transparent_mesh;

		World							*_world;

		float							_spawn_fade = 0;
	private:
		inline void	_setBlock(const ChunkLocalVec3i &pos, ChunkBlockStateId block)
		{
			ChunkBlockStateId	&b = _blocks[_getBlockIndex(pos)];

			if (b == 0 && block != 0)
				_non_air_blocks++;
			else if (b != 0 && block == 0)
				_non_air_blocks--;
			b = block;
		}
		inline void	_setBlockFromWorld(const ChunkWorldVec3i &world_pos, ChunkBlockStateId block)
		{
			ChunkWorldVec3i	chunk_block_pos = worldToChunkWorld(world_pos, CHUNK_SIZE);

			if (_pos == chunk_block_pos)
			{
				ChunkLocalVec3i	bp = worldToChunkLocal(world_pos, CHUNK_SIZE);
				_setBlock(bp, block);
			}
		}
		inline bool	_isInBounds(const ChunkLocalVec3i &pos)
		{
			return (pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < CHUNK_SIZE && pos.y < CHUNK_SIZE && pos.z < CHUNK_SIZE);
		}
		inline uint16_t	_getBlockIndex(const ChunkLocalVec3i &pos)
		{
			return (pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE * CHUNK_SIZE);
		}
};

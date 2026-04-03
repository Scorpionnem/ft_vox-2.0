#pragma once

#include "Window.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "World.hpp"
#include "ThreadPool.hpp"
#include <string>

class	App
{
	public:
		App() {}
		~App() {}

		void	run(void);
	private:
		Window	_window;

		Texture	_atlas;

		Shader	_terrain_shader;
		Shader	_selected_block_shader;

		World	_world;

		Camera	_cam;

		ThreadPool	_generation_threads;
	private:
		void	_place_block(BlockStateId block);
		void	_break_block(void);

		void	_loop(void);
		void	_init();
};

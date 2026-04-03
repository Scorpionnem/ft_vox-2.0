#pragma once

#include "Window.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
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
		Shader	_bounding_box_shader;
		Mesh	_cube_mesh;

		World	_world;

		Camera	_cam;

		ThreadPool	_generation_threads;

		bool		_ray_hit;
		Vec3i		_ray_hit_pos;
		Vec3i		_ray_prev_hit_pos;
	private:
		void	_cast_ray(const Window::Events &events);

		void	_loop(void);
		void	_init();
};

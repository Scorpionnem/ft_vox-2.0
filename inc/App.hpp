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

		bool	_show_debug = false;

		Vec3i	_render_distance = Vec3i(8);
		float	_fog_power = 4;
		bool	_fog_toggle = true;
		Vec3f	_sky_color = Vec3f(194 / 255.0, 235.0 / 255.0, 1.0);
		Vec3f	_fog_distance = Vec3f(256);
	private:
		void	_cast_ray(const Window::Events &events);

		void	_loop(void);
		void	_update(const Window::Events &events);
		void	_render(void);

		void	_imgui(const Window::Events &events);

		void	_init();
};

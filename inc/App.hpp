#pragma once

#include "Window.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Skybox.hpp"
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

		Chrono	_time;

		Skybox	_skybox;

		World	_world;

		Camera	_cam;
		std::shared_ptr<Entity>	_player_entity;

		ThreadPool	_generation_threads;

		bool		_ray_hit;
		Vec3i		_ray_hit_pos;
		Vec3i		_ray_prev_hit_pos;
		int			_break_anim_frame = -1;
		float		_break_anim_timer = 0;

		bool	_show_debug = false;

		std::vector<std::shared_ptr<Chunk>>	_vision;

		Vec3i	_render_distance = Vec3i(9);
		float	_fog_power = 4;
		bool	_fog_toggle = true;
		Vec3f	_fog_distance = Vec3f(288);
	private:
		void	updateCamera(Camera &cam, const Window::Events &events);

		void	_cast_ray(const Window::Events &events);

		void	_loop(void);
		void	_update(const Window::Events &events);
		void	_render(void);

		void	_imgui(const Window::Events &events);

		void	_init();
};

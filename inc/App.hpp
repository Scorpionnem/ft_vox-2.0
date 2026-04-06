#pragma once

#include "Window.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Skybox.hpp"
#include "ThreadPool.hpp"
#include "SelectedBlock.hpp"
#include <string>

class	App
{
	public:
		App() {}
		~App() {}

		void	run(void);
	private:
		Window	_window;
		Chrono	_time;

		Texture	_atlas;

		Shader	_bounding_box_shader;
		Mesh	_cube_mesh;

		Skybox			_skybox;

		Shader			_terrain_shader;
		World			_world;
		ThreadPool		_generation_threads;

		std::vector<std::shared_ptr<Chunk>>	_vision;

		SelectedBlock			_selected_block;
		Camera					_cam;
		std::shared_ptr<Entity>	_player_entity;

		bool	_show_debug = false;

		Vec3i	_render_distance = Vec3i(9);
		float	_fog_power = 4;
		bool	_fog_toggle = true;
		Vec3f	_fog_distance = Vec3f(288);
	private:
		void	_draw_bounding(const Vec3f &pos, const Vec3f &size, const Vec3f &color);
		void	updateCamera(Camera &cam, const Window::Events &events);

		void	_loop(void);
		void	_update(const Window::Events &events);
		void	_render(void);

		void	_imgui(const Window::Events &events);

		void	_init();
};

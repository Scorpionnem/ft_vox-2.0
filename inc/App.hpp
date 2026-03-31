#pragma once

#include "Window.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "World.hpp"
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

		World	_world;
	private:
		void	_loop(void);
		void	_init();
};

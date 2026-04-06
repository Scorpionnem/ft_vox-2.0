#pragma once

/*
	Skybox, also handles daynight cycle
*/

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Math.hpp"
#include "Camera.hpp"

class	Skybox
{
	public:
		Skybox() {}
		~Skybox() {}

		void	init();
		void	update();
		void	render(Camera &cam);
		void	imgui();

		Vec3f	get_fog_color()
		{
			return (_sky_down_color);
		}
	private:
		Shader	_skybox_shader;
		Mesh	_skybox_mesh;
		float	_sky_up_height = 0.0;
		float	_sky_mix_distance = 4;
		Vec3f	_sky_up_color = Vec3f(0.347368, 0.761233, 1.0);
		Vec3f	_sky_down_color = Vec3f(194 / 255.0, 235.0 / 255.0, 1.0);
};

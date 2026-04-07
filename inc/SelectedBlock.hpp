#pragma once

#include "World.hpp"
#include "Camera.hpp"
#include "Math.hpp"
#include "Window.hpp"

class	SelectedBlock
{
	public:
		SelectedBlock() {}
		~SelectedBlock() {}

		void	init();
		void	update(Camera &cam, World &world, const Window::Events &events);
		void	render(Camera &cam);

		bool	hit()
		{
			return (_hit);
		}
		Vec3i	hit_pos()
		{
			return (_hit_pos);
		}
		Vec3i	prev_hit_pos()
		{
			return (_prev_hit_pos);
		}
	private:
		bool		_hit;
		Vec3i		_hit_pos;
		Vec3i		_prev_hit_pos;

		Shader		_shader;
		Mesh		_mesh;

		int			_break_anim_frame = -1;
		float		_break_anim_timer = 0;
};

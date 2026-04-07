#include "SelectedBlock.hpp"

void	SelectedBlock::update(Camera &cam, World &world, const Window::Events &events)
{
	world.castRayToBlock(cam.pos, cam.front, 8, _hit, _hit_pos, _prev_hit_pos);

	(void)events;
	// _break_anim_timer += events.getDeltaTime();
	// if (_break_anim_timer >= 0.1)
	// {
	// 	_break_anim_frame++;
	// 	if (_break_anim_frame > 9)
	// 		_break_anim_frame = 0;
	// 	_break_anim_timer = 0;
	// }
}

void	SelectedBlock::render(Camera &cam)
{
	if (!_hit)
		return ;

	Mat4f	model = translate<float>(Vec3d(_hit_pos) - cam.pos);
	model = model * translate<float>(Vec3f(0.5));
	model = model * scale<float>(Vec3f(1.002));
	model = model * translate<float>(Vec3f(-0.5));

	_shader.use();
	_shader.setMat4f("view", cam.getViewMatrix());
	_shader.setMat4f("model", model);
	_shader.setMat4f("proj", cam.getProjectionMatrix());
	_shader.setInt("BREAK_ANIM_FRAME", _break_anim_frame);
	_mesh.draw();
}

void	SelectedBlock::init()
{
	_shader.load(GL_VERTEX_SHADER, "assets/shaders/selected_block.vs");
	_shader.load(GL_FRAGMENT_SHADER, "assets/shaders/selected_block.fs");
	_shader.link();
	_shader.setInt("atlas", 0);

	_mesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
	_mesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
	_mesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(Vertex, color));
	_mesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(Vertex, uv));
	_mesh.set_sizeof_layout(sizeof(Vertex));

	extern Face	FACE2[6];
	extern Face	FACE1[6];
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[0]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[1]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[2]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[3]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[4]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[5]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[0]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[1]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[2]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[3]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[4]), sizeof(Face));
	_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[5]), sizeof(Face));
	_mesh.upload();
}

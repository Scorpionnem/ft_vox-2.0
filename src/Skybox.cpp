#include "Skybox.hpp"

float SKYBOX_VERTICES[] = {
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,

	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f
};

void	Skybox::init()
{
	_skybox_shader.load(GL_VERTEX_SHADER, "assets/shaders/skybox.vs");
	_skybox_shader.load(GL_FRAGMENT_SHADER, "assets/shaders/skybox.fs");
	_skybox_shader.link();

	_skybox_mesh.add_vertex_layout(0, 3, GL_FLOAT, 0);
	_skybox_mesh.set_sizeof_layout(sizeof(Vec3f));

	_skybox_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&SKYBOX_VERTICES), sizeof(SKYBOX_VERTICES));
	_skybox_mesh.upload();
}

void	Skybox::update()
{

}

void	Skybox::render(Camera &cam)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	Mat4f	model = Mat4f(1.0);
	Mat4f	view_no_translate = cam.getViewMatrix();
	view_no_translate.data[12] = 0;
	view_no_translate.data[13] = 0;
	view_no_translate.data[14] = 0;
	view_no_translate.data[15] = 1;

	_skybox_shader.use();
	_skybox_shader.setMat4f("view", view_no_translate);
	_skybox_shader.setMat4f("proj", cam.getProjectionMatrix());
	_skybox_shader.setMat4f("model", model);
	_skybox_shader.setVec3f("SKYBOX_UP_COLOR", _sky_up_color);
	_skybox_shader.setVec3f("SKYBOX_DOWN_COLOR", _sky_down_color);
	_skybox_shader.setFloat("SKYBOX_UP_HEIGHT", _sky_up_height);
	_skybox_shader.setFloat("SKYBOX_MIX_DISTANCE", _sky_mix_distance);

	_skybox_mesh.draw();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

#include <imgui.h>

void	Skybox::imgui()
{
	if (ImGui::Begin("skybox / fog"))
	{
		ImGui::InputFloat("Sky up height", &_sky_up_height);
		ImGui::InputFloat("Sky mix distance", &_sky_mix_distance);

		// ImGui::InputFloat("Fog Power", &_fog_power);
		// ImGui::Checkbox("Toggle Fog", &_fog_toggle);
		// ImGui::SliderFloat3("Fog Distance", &_fog_distance.x, 0, max(_render_distance * CHUNK_SIZE + CHUNK_SIZE));
		// _fog_power = std::clamp(_fog_power, 1.0f, 16.0f);

		ImGui::ColorPicker3("sky down color", &_sky_down_color.x);
		ImGui::ColorPicker3("sky up color", &_sky_up_color.x);
	}
	ImGui::End();
}

#include "App.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "Biome.hpp"

#include <stdexcept>
#include <iostream>

#include <imgui.h>

void	App::updateCamera(Camera &cam, const Window::Events &events)
{
	float	speed = 5 * events.getDeltaTime();
	float	sensibility = 50 * events.getDeltaTime();

	if (events.getKey(SDLK_LCTRL))
		speed = 50 * events.getDeltaTime();

	_player_entity->velocity = 0;

	if (events.getKey(SDLK_w))
		_player_entity->velocity = _player_entity->velocity + (cam.front * speed);
	if (events.getKey(SDLK_s))
		_player_entity->velocity = _player_entity->velocity + -1.0 * (cam.front * speed);
	if (events.getKey(SDLK_SPACE))
		_player_entity->velocity = _player_entity->velocity + (cam.up * speed);
	if (events.getKey(SDLK_LSHIFT))
		_player_entity->velocity = _player_entity->velocity + -1.0 * (cam.up * speed);
	if (events.getKey(SDLK_a))
		_player_entity->velocity = _player_entity->velocity + -1.0 * (normalize(cross(cam.front, cam.up)) * speed);
	if (events.getKey(SDLK_d))
		_player_entity->velocity = _player_entity->velocity + (normalize(cross(cam.front, cam.up)) * speed);
	if (events.getKey(SDLK_UP))
		cam.pitch += sensibility * 2;
	if (events.getKey(SDLK_DOWN))
		cam.pitch -= sensibility * 2;
	if (events.getKey(SDLK_RIGHT))
		cam.yaw += sensibility * 2;
	if (events.getKey(SDLK_LEFT))
		cam.yaw -= sensibility * 2;

	_player_entity->update(events.getDeltaTime());

	cam.pos = _player_entity->get_eye();
	cam.update(events.getDeltaTime(), events.getAspectRatio());
}

void	App::_update(const Window::Events &events)
{
	if (events.getKeyPressed(SDLK_F3))
		_show_debug = !_show_debug;

	updateCamera(_cam, events);

	_world.setUpdateCenter(_cam.pos);
	_world.update(_generation_threads, events.getDeltaTime());

	_selected_block.update(_cam, _world, events);
}

void	App::_render(void)
{
	_skybox.render(_cam);

	_atlas.bind(0);
	_terrain_shader.use();
	_terrain_shader.setMat4f("view", _cam.getViewMatrix());
	_terrain_shader.setMat4f("proj", _cam.getProjectionMatrix());
	_terrain_shader.setVec3f("FOG_COLOR", _skybox.get_fog_color());
	_terrain_shader.setVec3f("FOG_DISTANCE", _fog_distance);
	_terrain_shader.setFloat("FOG_POWER", _fog_power);
	_terrain_shader.setBool("FOG_TOGGLE", _fog_toggle);

	_vision = _world.getVision(_cam, _render_distance);
	for (auto &chunk : _vision)
	{
		chunk->draw(_terrain_shader, _cam.pos);
		if (_show_debug)
		{
			_bounding_box_shader.use();
			_bounding_box_shader.setMat4f("model", translate<float>(Vec3d(chunk->pos() * CHUNK_SIZE) - _cam.pos) * scale<float>(Vec3f(CHUNK_SIZE)));
			_bounding_box_shader.setMat4f("view", _cam.getViewMatrix());
			_bounding_box_shader.setMat4f("proj", _cam.getProjectionMatrix());
			_cube_mesh.draw();
		}
	}

	_selected_block.render(_cam);
}

uint64_t	DRAW_CALLS = 0;

void	App::_imgui(const Window::Events &events)
{
	if (_show_debug)
	{
		_world.imgui();
		_cam.imgui();
		_generation_threads.imgui();
		_skybox.imgui();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		if (ImGui::Begin("ft_minecraft", nullptr, ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("FPS: %.2f", 1.0 / events.getDeltaTime());
			ImGui::Text("Draw calls: %lu", DRAW_CALLS);

			ImGui::InputInt3("Render distance", &_render_distance.x);
		}
		ImGui::End();
		if (ImGui::Begin("generation"))
		{
			Vec2f	p = Vec2f(_cam.pos.x, _cam.pos.z);
			ImGui::Text("C: %.2f", Biome::get_continentalness(p));
			ImGui::Text("E: %.2f", Biome::get_erosion(p));
			ImGui::Text("T: %.2f", Biome::get_temperature(p));
		}
		ImGui::End();

	}
}

void	App::_loop(void)
{
	while (_window.is_open())
	{
		const Window::Events	&events = _window.pollEvents();
		if (events.getKey(SDLK_ESCAPE))
		{
			_window.close();
			break ;
		}

		_update(events);

		_render();

		_imgui(events);

		_window.render();

		DRAW_CALLS = 0;
	}
}

#include "PlainsBiome.hpp"
#include "MountainPeaksBiome.hpp"
#include "MountainLowBiome.hpp"
#include "ForestBiome.hpp"
#include "BeachBiome.hpp"
#include "GravelBeachBiome.hpp"
#include "ShallowOceanBiome.hpp"
#include "OceanBiome.hpp"
#include "DeepOceanBiome.hpp"
#include "PlateauBiome.hpp"
#include "OldMountainBiome.hpp"
#include "DesertBiome.hpp"
#include "MesaBiome.hpp"
#include "MesaPlateauBiome.hpp"
#include "MesaTransitionBiome.hpp"

void	App::_init()
{
	_time.start();

	_window.open("ft_vox", 1024, 768);

	_generation_threads.add(8);

	_skybox.init();
	_selected_block.init();

	#define HITBOX_WIDTH 0.8
	#define HITBOX_HEIGHT 1.8
	#define CAMERA_HEIGHT 1.6
	_player_entity = std::make_shared<Entity>(_world, Vec3d(0, 120, 0), Vec3f(HITBOX_WIDTH, HITBOX_HEIGHT, HITBOX_WIDTH), Vec3f(0, HITBOX_HEIGHT / 2, 0), Vec3f(0, CAMERA_HEIGHT, 0));

	_atlas.load("assets/textures/atlas.png");
	_atlas.upload();

	_terrain_shader.load(GL_VERTEX_SHADER, "assets/shaders/terrain.vs");
	_terrain_shader.load(GL_FRAGMENT_SHADER, "assets/shaders/terrain.fs");
	_terrain_shader.link();
	_terrain_shader.setInt("atlas", 0);

	_bounding_box_shader.load(GL_VERTEX_SHADER, "assets/shaders/bounding_box.vs");
	_bounding_box_shader.load(GL_FRAGMENT_SHADER, "assets/shaders/bounding_box.fs");
	_bounding_box_shader.link();
	_bounding_box_shader.setInt("atlas", 0);

	_cube_mesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
	_cube_mesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
	_cube_mesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(Vertex, color));
	_cube_mesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(Vertex, uv));
	_cube_mesh.set_sizeof_layout(sizeof(Vertex));

	extern Face	FACE2[6];
	extern Face	FACE1[6];
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[0]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[1]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[2]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[3]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[4]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE2[5]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[0]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[1]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[2]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[3]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[4]), sizeof(Face));
	_cube_mesh.add_triangle_data(reinterpret_cast<uint8_t*>(&FACE1[5]), sizeof(Face));
	_cube_mesh.upload();

	extern std::vector<std::shared_ptr<Biome>>	ALL_BIOMES;

	ALL_BIOMES.push_back(std::make_shared<DesertBiome>());

	ALL_BIOMES.push_back(std::make_shared<PlainsBiome>());
	ALL_BIOMES.push_back(std::make_shared<ForestBiome>());


	ALL_BIOMES.push_back(std::make_shared<BeachBiome>());
	ALL_BIOMES.push_back(std::make_shared<GravelBeachBiome>());


	ALL_BIOMES.push_back(std::make_shared<ShallowOceanBiome>());
	ALL_BIOMES.push_back(std::make_shared<OceanBiome>());
	ALL_BIOMES.push_back(std::make_shared<DeepOceanBiome>());


	ALL_BIOMES.push_back(std::make_shared<MountainLowBiome>());
	ALL_BIOMES.push_back(std::make_shared<PlateauBiome>());
	ALL_BIOMES.push_back(std::make_shared<MesaPlateauBiome>());
	ALL_BIOMES.push_back(std::make_shared<MesaBiome>());
	ALL_BIOMES.push_back(std::make_shared<MesaTransitionBiome>());


	ALL_BIOMES.push_back(std::make_shared<MountainPeaksBiome>());
	ALL_BIOMES.push_back(std::make_shared<OldMountainBiome>());
}

void	App::run(void)
{
	_init();

	_loop();
}

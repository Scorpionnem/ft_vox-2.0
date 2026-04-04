#include "App.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "Biome.hpp"

#include <stdexcept>
#include <iostream>

#include <imgui.h>

void	updateCamera(Camera &cam, const Window::Events &events)
{
	float	speed = 5 * events.getDeltaTime();
	float	sensibility = 50 * events.getDeltaTime();

	if (events.getKey(SDLK_LCTRL))
		speed = 50 * events.getDeltaTime();

	if (events.getKey(SDLK_w))
		cam.pos = cam.pos + cam.front * speed;
	if (events.getKey(SDLK_s))
		cam.pos = cam.pos - cam.front * speed;
	if (events.getKey(SDLK_SPACE))
		cam.pos = cam.pos + cam.up * speed;
	if (events.getKey(SDLK_LSHIFT))
		cam.pos = cam.pos - cam.up * speed;
	if (events.getKey(SDLK_a))
		cam.pos = cam.pos - normalize(cross(cam.front, cam.up)) * speed;
	if (events.getKey(SDLK_d))
		cam.pos = cam.pos + normalize(cross(cam.front, cam.up)) * speed;
	if (events.getKey(SDLK_UP))
		cam.pitch += sensibility * 2;
	if (events.getKey(SDLK_DOWN))
		cam.pitch -= sensibility * 2;
	if (events.getKey(SDLK_RIGHT))
		cam.yaw += sensibility * 2;
	if (events.getKey(SDLK_LEFT))
		cam.yaw -= sensibility * 2;

	cam.update(events.getDeltaTime(), events.getAspectRatio());
}

void	App::_cast_ray(const Window::Events &events)
{
	_world.castRayToBlock(_cam.pos, _cam.front, 8, _ray_hit, _ray_hit_pos, _ray_prev_hit_pos);

	if (_ray_hit)
	{
		Mat4f	model = translate<float>(_ray_hit_pos);
		model = model * translate<float>(Vec3f(0.5));
		model = model * scale<float>(Vec3f(1.002));
		model = model * translate<float>(Vec3f(-0.5));

		_selected_block_shader.use();
		_selected_block_shader.setMat4f("view", _cam.getViewMatrix());
		_selected_block_shader.setMat4f("model", model);
		_selected_block_shader.setMat4f("proj", perspective<float>(90, events.getAspectRatio(), 0.1, 1000.0));
		_cube_mesh.draw();

		if (events.getMouseBtnPressed(SDL_BUTTON_LEFT))
		{
			auto	chunk = _world.getChunk(worldToChunkWorld(_ray_hit_pos, CHUNK_SIZE));
			if (chunk && chunk->state() >= Chunk::State::GENERATED)
			{
				chunk->setBlock(worldToChunkLocal(_ray_hit_pos, CHUNK_SIZE), BLOCK_AIR);
				if (chunk->state() >= Chunk::State::MESHED)
				{
					chunk->mesh();
					chunk->mesh_neighbours();
				}
			}
		}
		if (events.getMouseBtnPressed(SDL_BUTTON_RIGHT))
		{
			auto	chunk = _world.getChunk(worldToChunkWorld(_ray_prev_hit_pos, CHUNK_SIZE));
			if (chunk && chunk->state() >= Chunk::State::GENERATED)
			{
				chunk->setBlock(worldToChunkLocal(_ray_prev_hit_pos, CHUNK_SIZE), BLOCK_COBBLESTONE);
				if (chunk->state() >= Chunk::State::MESHED)
				{
					chunk->mesh();
					chunk->mesh_neighbours();
				}
			}
		}
	}
}

extern std::vector<std::shared_ptr<Biome>>	ALL_BIOMES;

void	App::_loop(void)
{
	_cam.pos.x = 0;
	_cam.pos.y = 120;
	_cam.pos.z = 0;

	bool	showDebug = false;
	float	fog_power = 4;
	bool	fog_toggle = true;
	Vec3f	sky_color(194 / 255.0, 235.0 / 255.0, 1.0);
	Vec3f	render_distance = Vec3f(8);
	Vec3f	fog_distance = render_distance * CHUNK_SIZE;

	while (_window.is_open())
	{
		const Window::Events	&events = _window.pollEvents();
		if (events.getKey(SDLK_ESCAPE))
		{
			_window.close();
			break ;
		}

		updateCamera(_cam, events);

		_world.setUpdateCenter(_cam.pos);
		_world.update(_generation_threads, events.getDeltaTime());

		auto	view = _world.getVision(_cam, render_distance);

		_atlas.bind(0);

		_terrain_shader.use();
		_terrain_shader.setMat4f("view", _cam.getViewMatrix());
		_terrain_shader.setMat4f("proj", perspective<float>(90, events.getAspectRatio(), 0.1, 1000.0));

		_terrain_shader.setVec3f("RENDER_DISTANCE", render_distance * CHUNK_SIZE);
		_terrain_shader.setVec3f("VIEW_POS", _cam.pos);
		_terrain_shader.setFloat("FOG_POWER", fog_power);
		_terrain_shader.setVec3f("FOG_COLOR", sky_color);
		_terrain_shader.setBool("FOG_TOGGLE", fog_toggle);
		_terrain_shader.setInt("CHUNK_SIZE", CHUNK_SIZE);
		_terrain_shader.setVec3f("FOG_DISTANCE", fog_distance);

		glClearColor(sky_color.x, sky_color.y, sky_color.z, 1.0);

		for (auto chunk : view)
		{
			if (chunk->state() < Chunk::State::UPLOADED)
				chunk->upload();
			chunk->draw(_terrain_shader);

			if (showDebug)
			{
				_bounding_box_shader.use();
				_bounding_box_shader.setMat4f("model", translate<float>(chunk->pos() * CHUNK_SIZE) * scale<float>(Vec3f(CHUNK_SIZE)));
				_bounding_box_shader.setMat4f("view", _cam.getViewMatrix());
				_bounding_box_shader.setMat4f("proj", perspective<float>(90, events.getAspectRatio(), 0.1, 1000.0));
				_cube_mesh.draw();
			}
		}

		_cast_ray(events);

		if (events.getKeyPressed(SDLK_F3))
			showDebug = !showDebug;
		if (showDebug)
		{
			_world.imgui();
			_cam.imgui();
			_generation_threads.imgui();
			if (ImGui::Begin("ft_vox"))
			{
				ImGui::Text("FPS: %.2f", 1.0 / events.getDeltaTime());
				ImGui::InputFloat("Fog Power", &fog_power);
				ImGui::Checkbox("Toggle Fog", &fog_toggle);
				ImGui::SliderFloat3("Fog Distance", &fog_distance.x, 0, max(render_distance * CHUNK_SIZE + CHUNK_SIZE));
				fog_power = std::clamp(fog_power, 1.0f, 16.0f);
				ImGui::ColorPicker3("sky color", &sky_color.x);
			}
			ImGui::End();

			if (ImGui::Begin("generation"))
			{
				Vec2f	p = {_cam.pos.x, _cam.pos.z};
				ImGui::Text("C: %.2f", Biome::get_continentalness(p));
				ImGui::Text("E: %.2f", Biome::get_erosion(p));
				ImGui::Text("T: %.2f", Biome::get_temperature(p));
			}
			ImGui::End();
		}

		_window.render();
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
	_window.open("ft_vox", 1024, 768);

	_atlas.load("assets/textures/atlas.png");
	_atlas.upload();

	_terrain_shader.load(GL_VERTEX_SHADER, "assets/shaders/mesh.vs");
	_terrain_shader.load(GL_FRAGMENT_SHADER, "assets/shaders/mesh.fs");
	_terrain_shader.link();
	_terrain_shader.setInt("atlas", 0);

	_selected_block_shader.load(GL_VERTEX_SHADER, "assets/shaders/selected_block.vs");
	_selected_block_shader.load(GL_FRAGMENT_SHADER, "assets/shaders/selected_block.fs");
	_selected_block_shader.link();
	_selected_block_shader.setInt("atlas", 0);

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

	_generation_threads.add(8);

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

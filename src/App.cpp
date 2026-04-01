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
	float	speed = 50 * events.getDeltaTime();
	float	sensibility = 50 * events.getDeltaTime();

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

extern std::vector<std::shared_ptr<Biome>>	ALL_BIOMES;

void	App::_loop(void)
{
	Camera	cam;

	cam.pos.x = 0;
	cam.pos.z = 0;
	cam.pos.y = 130;

	ThreadPool	genThreads;
	genThreads.add(8);

	bool	showDebug = false;
	float	fog_power = 4;
	bool	fog_toggle = true;
	Vec3f	sky_color(194 / 255.0, 235.0 / 255.0, 1.0);
	Vec3f	render_distance = Vec3f(8);
	Vec3f	fog_distance = render_distance * CHUNK_SIZE + CHUNK_SIZE;

	while (_window.is_open())
	{
		const Window::Events	&events = _window.pollEvents();
		if (events.getKey(SDLK_ESCAPE))
		{
			_window.close();
			break ;
		}

		updateCamera(cam, events);

		_world.setUpdateCenter(cam.pos);
		_world.update(genThreads, events.getDeltaTime());

		auto	view = _world.getVision(cam, render_distance);

		_atlas.bind(0);

		_terrain_shader.use();
		_terrain_shader.setMat4f("view", cam.getViewMatrix());
		_terrain_shader.setMat4f("proj", perspective<float>(90, events.getAspectRatio(), 0.1, 1000.0));

		_terrain_shader.setVec3f("RENDER_DISTANCE", render_distance * CHUNK_SIZE);
		_terrain_shader.setVec3f("VIEW_POS", cam.pos);
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
		}

		if (events.getKeyPressed(SDLK_F3))
			showDebug = !showDebug;
		if (showDebug)
		{
			_world.imgui();
			cam.imgui();
			genThreads.imgui();
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
				Vec2f	p = {cam.pos.x, cam.pos.z};
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

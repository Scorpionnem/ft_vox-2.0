#include "App.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "World.hpp"

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

void	App::_loop(void)
{
	Camera	cam;

	World	world;

	ThreadPool	genThreads;
	genThreads.add(8);

	Shader	shader;
	shader.load(GL_VERTEX_SHADER, "assets/shaders/mesh.vs");
	shader.load(GL_FRAGMENT_SHADER, "assets/shaders/mesh.fs");
	shader.link();

	shader.setInt("atlas", 0);

	Texture	texture;
	texture.load("assets/textures/atlas.png");
	texture.upload();

	bool	showDebug = false;
	float	fog_power = 4;
	bool	fog_toggle = true;
	Vec3f	sky_color(194 / 255.0, 235.0 / 255.0, 1.0);
	Vec3f	render_distance = Vec3f(4);
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

		world.setUpdateCenter(cam.pos);
		world.update(genThreads, events.getDeltaTime());

		auto	view = world.getVision(cam, render_distance);

		texture.bind(0);

		shader.use();
		shader.setMat4f("view", cam.getViewMatrix());
		shader.setMat4f("proj", perspective<float>(90, events.getAspectRatio(), 0.1, 1000.0));

		shader.setVec3f("RENDER_DISTANCE", render_distance * CHUNK_SIZE);
		shader.setVec3f("VIEW_POS", cam.pos);
		shader.setFloat("FOG_POWER", fog_power);
		shader.setVec3f("FOG_COLOR", sky_color);
		shader.setBool("FOG_TOGGLE", fog_toggle);
		shader.setInt("CHUNK_SIZE", CHUNK_SIZE);
		shader.setVec3f("FOG_DISTANCE", fog_distance);

		glClearColor(sky_color.x, sky_color.y, sky_color.z, 1.0);

		for (auto chunk : view)
		{
			if (chunk->state() < Chunk::State::UPLOADED)
				chunk->upload();
			chunk->draw(shader);
		}

		if (events.getKeyPressed(SDLK_F3))
			showDebug = !showDebug;
		if (showDebug)
		{
			world.imgui();
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
		}

		_window.render();
	}
}

void	App::_init()
{
	_window.open("ft_vox", 1024, 768);
}

void	App::run(void)
{
	_init();

	_loop();
}

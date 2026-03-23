#include "Window.hpp"

#include <glad/glad.h>

#include <iostream>
#include <memory>
#include <string>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

void	Window::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(_window);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void	Window::open(const char *title, uint32_t width, uint32_t height)
{
	_width = width;
	_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		throw (std::runtime_error(SDL_GetError()));

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	_window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_width, _height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!_window)
	{
		SDL_Quit();
		throw (std::runtime_error(SDL_GetError()));
	}

	_GLContext = SDL_GL_CreateContext(_window);
	if (!_GLContext)
	{
		SDL_DestroyWindow(_window);
		SDL_Quit();
		throw (std::runtime_error(SDL_GetError()));
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_GL_DeleteContext(_GLContext);
		SDL_DestroyWindow(_window);
		SDL_Quit();
		throw (std::runtime_error("Failed to initialize GLAD"));
	}

	glViewport(0, 0, _width, _height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(_window, _GLContext);
	ImGui_ImplOpenGL3_Init();

	_open = true;
	_chrono.start();
	_lastFrame = _chrono.get();
}

const Window::Events	&Window::pollEvents()
{
	SDL_Event	event;

	_events._reset();
	_events._deltaTime = _chrono.get() - _lastFrame;
	_events._aspectRatio = (double)_width / (double)_height;
	_lastFrame = _chrono.get();
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				_open = false;
				break ;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					_width = event.window.data1;
					_height = event.window.data2;
					glViewport(0, 0, _width, _height);
				}
				break ;
			case SDL_KEYDOWN:
				_events._keys[event.key.keysym.sym] = true;
				_events._keysPressed[event.key.keysym.sym] = true;
				break ;
			case SDL_KEYUP:
				_events._keys[event.key.keysym.sym] = false;
				_events._keysReleased[event.key.keysym.sym] = true;
				break ;
			case SDL_MOUSEMOTION:
				_events._mouseDeltaX = event.motion.xrel;
				_events._mouseDeltaY = event.motion.yrel;
				break ;
			case SDL_MOUSEBUTTONDOWN:
				_events._mouseBtn[event.button.button] = true;
				_events._mouseBtnPressed[event.button.button] = true;
				break ;
			case SDL_MOUSEBUTTONUP:
				_events._mouseBtn[event.button.button] = false;
				_events._mouseBtnPressed[event.button.button] = true;
				break ;
			case SDL_MOUSEWHEEL:
				_events._mouseScroll = event.wheel.y;
				break ;
		}
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	return (_events);
}

void	Window::Events::_reset()
{
	_mouseDeltaX = 0;
	_mouseDeltaY = 0;
	_mouseScroll = 0;
	_keysPressed.clear();
	_keysReleased.clear();
	_mouseBtnPressed.clear();
	_mouseBtnLifted.clear();
}

bool	Window::Events::getKey(int key) const
{
	auto find = _keys.find(key);
	if (find == _keys.end())
		return (false);
	return (find->second);
}

bool	Window::Events::getMouseBtn(int key) const
{
	auto find = _mouseBtn.find(key);
	if (find == _mouseBtn.end())
		return (false);
	return (find->second);
}

bool	Window::Events::getMouseBtnPressed(int key) const
{
	auto find = _mouseBtnPressed.find(key);
	if (find == _mouseBtnPressed.end())
		return (false);
	return (find->second);
}

bool	Window::Events::getMouseBtnLifted(int key) const
{
	auto find = _mouseBtnLifted.find(key);
	if (find == _mouseBtnLifted.end())
		return (false);
	return (find->second);
}

bool	Window::Events::getKeyPressed(int key) const
{
	auto find = _keysPressed.find(key);
	if (find == _keysPressed.end())
		return (false);
	return (find->second);
}

bool	Window::Events::getKeyReleased(int key) const
{
	auto find = _keysReleased.find(key);
	if (find == _keysReleased.end())
		return (false);
	return (find->second);
}

float	Window::Events::getMouseScroll() const
{
	return (_mouseScroll);
}

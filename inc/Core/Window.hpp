#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <map>

#include "Vec3.hpp"
#include "Vec2.hpp"
#include "Chrono.hpp"

class	Window
{
	public:
		class	Events
		{
			public:
				Events() {}
				~Events() {}

				bool	getKey(int key) const;
				bool	getMouseBtn(int key) const;
				bool	getMouseBtnPressed(int key) const;
				bool	getMouseBtnLifted(int key) const;
				bool	getKeyPressed(int key) const;
				bool	getKeyReleased(int key) const;
				float	getMouseScroll() const;

				double	getDeltaTime() const {return (_deltaTime);}
				double	getAspectRatio() const {return (_aspectRatio);}
			private:
				friend Window;

				void	_reset();

				double	_deltaTime = 0;
				double	_aspectRatio = 0;

				float	_mouseDeltaX = 0;
				float	_mouseDeltaY = 0;
				float	_mouseScroll = 0;
				std::map<int, bool>	_keys;
				std::map<int, bool>	_keysPressed;
				std::map<int, bool>	_keysReleased;
				std::map<int, bool>	_mouseBtn;
				std::map<int, bool>	_mouseBtnPressed;
				std::map<int, bool>	_mouseBtnLifted;
		};
	public:
		Window() {}
		~Window()
		{
			SDL_DestroyWindow(_window);
			SDL_Quit();

			_open = false;
		}

		void	open(const char *title, uint32_t width, uint32_t height);
		void	close() {_open = false;}

		const Window::Events	&pollEvents();
		void	clearWindow()
		{
		}
		void	render();

		bool	is_open() {return (_open);}
	private:
		bool		_open = false;
		SDL_Window	*_window = NULL;
		SDL_GLContext	_GLContext = NULL;

		uint32_t	_width;
		uint32_t	_height;

		Window::Events	_events;

		Chrono	_chrono;
		double	_lastFrame = 0;
};

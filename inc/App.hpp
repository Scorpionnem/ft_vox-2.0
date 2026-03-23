#pragma once

#include "Window.hpp"
#include <string>

class	App
{
	public:
		App() {}
		~App() {}

		void	run(void);
	private:
		Window	_window;
	private:
		void	_loop(void);
		void	_init();
};

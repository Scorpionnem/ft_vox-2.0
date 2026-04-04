#include "App.hpp"

#include <stdexcept>
#include <iostream>

int	main(void)
{
	try
	{
		App	app;

		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "ft_vox: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}

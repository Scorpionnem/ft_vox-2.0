#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

void	Shader::use()
{
	glUseProgram(_id);
}

void	Shader::load(GLenum type, const std::string &path)
{
	if (_id == 0)
		_id = glCreateProgram();

	_files[type] = path;

	std::string	file = _processShaderFile(path);
	const char	*rawFile = file.c_str();

	uint32_t	shader = glCreateShader(type);

	glShaderSource(shader, 1, &rawFile, NULL);
	glCompileShader(shader);

	int				success;
	char			infoLog[4096];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << std::endl;
		throw (std::runtime_error("Failed to load shader"));
	}
	glAttachShader(_id, shader);
}

void	Shader::link()
{
	int				success;
	char			infoLog[4096];

	glLinkProgram(_id);
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << std::endl;
		throw (std::runtime_error("Failed to link shader"));
	}
}

void	Shader::setInt(const std::string &name, int val)
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), val);
}

void	Shader::setBool(const std::string &name, bool val)
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), val);
}

void	Shader::setFloat(const std::string &name, float val)
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), val);
}

void	Shader::setVec3f(const std::string &name, Vec3f val)
{
	glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &val.x);
}

void	Shader::setMat4f(const std::string &name, Mat4f val)
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, val.data);
}

std::string	Shader::_processShaderFile(const std::string &path, bool rec)
{
	std::ifstream	file(path);
	std::string		res;

	if (!file.is_open())
		throw std::runtime_error("Could not open " + path);

	std::string	line;
	while (std::getline(file, line))
	{
		#define MBATTY_IMPORT "#mbatty_import"
		if (!line.compare(0, sizeof(MBATTY_IMPORT) - 1, MBATTY_IMPORT))
		{
			if (rec)
				throw std::runtime_error("Recursive #mbatty_import: " + line);
			size_t first = line.find('"');
			size_t last = line.rfind('"');

			if (first != std::string::npos && last != std::string::npos && first < last)
			{
				std::string	import_path = line.substr(first + 1, last - first - 1);

				res += _processShaderFile(import_path, true);
			}
			else
				throw std::runtime_error("Invalid #mbatty_import: " + line);
		}
		else
			res += line + "\n";
	}
	return (res);
}

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <glad/glad.h>

#include "Math.hpp"

class	Shader
{
	public:
		Shader() {}
		~Shader() {unlink();}

		void	use();
		void	load(GLenum type, const std::string &path);
		void	reload()
		{
			unlink();
			for (const auto &file : _files)
				load(file.first, file.second);
			link();
		}
		void	link();
		void	unlink()
		{
			if (_id != 0)
				glDeleteProgram(_id);
			_id = 0;
		}
		void	setInt(const std::string &name, int val);
		void	setFloat(const std::string &name, float val);
		void	setVec3f(const std::string &name, Vec3f val);
		void	setMat4f(const std::string &name, Mat4f val);
	private:
		std::string	_processShaderFile(const std::string &path);
		std::map<GLenum, std::string>	_files;
		uint32_t	_id = 0;
};

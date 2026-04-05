#pragma once

#include <glad/glad.h>
#include <vector>

class	Mesh
{
	private:
		struct	VertexLayout
		{
			// Location (layout (location = X))
			uint32_t	location;

			// Number of .type in the layout (3 for a vec3)
			uint32_t	count;
			// Type of the layout (GL_FLOAT for a vec3)
			GLenum		type;

			// Offset of layout in struct
			uint64_t	offset;
		};
	public:
		Mesh() {}
		~Mesh()
		{
			if (_VAO != 0)
				glDeleteVertexArrays(1, &_VAO);
			if (_VBO != 0)
				glDeleteBuffers(1, &_VBO);
		}
		void	add_triangle_data(uint8_t *bytes, uint64_t size)
		{
			uint64_t vertices_in_bytes = size / _sizeof_layout;
			_triangles += vertices_in_bytes;
			_mesh_bytes.insert(_mesh_bytes.end(), bytes, bytes + size);
		}
		void	add_vertex_layout(uint32_t location, uint32_t count, uint32_t type, uint32_t offset)
		{
			_vertex_layouts.push_back(VertexLayout{.location = location, .count = count, .type = type, .offset = offset});
		}
		void	set_sizeof_layout(uint64_t size)
		{
			_sizeof_layout = size;
		}
		void	clear_data()
		{
			_mesh_bytes.clear();
			_triangles = 0;
		}
		void	upload()
		{
			if (_VAO != 0)
				glDeleteVertexArrays(1, &_VAO);
			if (_VBO != 0)
				glDeleteBuffers(1, &_VBO);

			glGenVertexArrays(1, &_VAO);
			glGenBuffers(1, &_VBO);

			glBindVertexArray(_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, _mesh_bytes.size(), _mesh_bytes.data(), GL_STATIC_DRAW);

			for (auto &v : _vertex_layouts)
			{
				glVertexAttribPointer(v.location, v.count, v.type, GL_FALSE, _sizeof_layout, (void*)v.offset);
				glEnableVertexAttribArray(v.location);
			}

			glBindVertexArray(0);

			_uploaded_triangles = _triangles;

			clear_data();
		}
		void	draw()
		{
			if (_uploaded_triangles == 0)
				return ;

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, _uploaded_triangles);
			glBindVertexArray(0);

			extern uint64_t	DRAW_CALLS;
			DRAW_CALLS++;
		}

		uint64_t	drawn_triangles()
		{
			return (std::max(_triangles, _uploaded_triangles));
		}
	private:
		uint32_t				_VAO = 0;
		uint32_t				_VBO = 0;

		std::vector<uint8_t>		_mesh_bytes;
		uint64_t					_triangles = 0;
		uint64_t					_uploaded_triangles = 0;

		uint32_t					_sizeof_layout = 0;
		std::vector<VertexLayout>	_vertex_layouts;
};


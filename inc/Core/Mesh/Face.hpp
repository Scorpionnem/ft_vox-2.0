#pragma once

#include "Math.hpp"

struct	Vertex
{
	constexpr Vertex(){}
	constexpr Vertex(Vec3f pos, Vec3f normal, Vec2f uv)
	{
		this->pos = pos;
		this->normal = normal;
		this->uv = uv;
	}
	Vec3f	pos;
	Vec3f	normal;
	Vec3f	color;
	Vec2f	uv;
};

struct	Face
{
	constexpr Face(){}
	constexpr Face(Vertex v1, Vertex v2, Vertex v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
	Vertex	v1;
	Vertex	v2;
	Vertex	v3;
};

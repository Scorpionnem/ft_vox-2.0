#pragma once

#include "World.hpp"

Vec3f	_solve_collisions(Vec3f pos, Vec3f size, Vec3f velocity, World &world);

class	Entity
{
	public:
		Entity(World &world, Vec3d pos, Vec3f hitbox_size, Vec3f hitbox_offset, Vec3f eye = Vec3f(0))
		{
			_world = &world;
			this->pos = pos;
			_hitbox_size = hitbox_size;
			_hitbox_offset = hitbox_offset;
			_eye = eye;
		}
		~Entity() {}

		void	update(float delta)
		{
			(void)delta;

			Vec3f	hitbox_pos = Vec3f(pos.x + _hitbox_offset.x, pos.y + _hitbox_offset.y, pos.z + _hitbox_offset.z);

			velocity = _solve_collisions(hitbox_pos, _hitbox_size, velocity, *_world);
			pos = pos + velocity;
		}
		Vec3f	get_eye()
		{
			return (pos + _eye);
		}
	public:
		Vec3d	pos;
		Vec3d	velocity;
	private:
		// Offset to the eye (offset from pos)
		Vec3f	_eye;
		Vec3f	_hitbox_size;
		// hitbox_pos = Vec3f(pos.x + _hitbox_offset.x, pos.y + _hitbox_offset.y, pos.z + _hitbox_offset.z)
		Vec3f	_hitbox_offset;

		World	*_world;
};

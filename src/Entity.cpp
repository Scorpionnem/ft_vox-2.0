#include "Entity.hpp"

void	Entity::update(float delta)
{
	(void)delta;

	Vec3f	hitbox_pos = Vec3f(pos.x + _hitbox_offset.x, pos.y + _hitbox_offset.y, pos.z + _hitbox_offset.z);

	velocity = _solve_collisions(hitbox_pos, _hitbox_size, velocity, *_world);
	pos = pos + velocity;
}

void	Entity::render()
{

}

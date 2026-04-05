/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 16:04:06 by mbatty            #+#    #+#             */
/*   Updated: 2026/04/05 12:49:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Math.hpp"

#include <iostream>

class	Camera
{
	private:
		struct	Plane
		{
			float	A, B, C, D;
			void	normalize();
			Vec3f	getNormal() const
			{
				return (Vec3f(A, B, C));
			}
			float	getDist(Vec3f point) const
			{
				return (dot(getNormal(), point) + D);
			}
		};
		struct	Frustum
		{
			Plane top, bottom, right, left, zNear, zFar;
			bool	isInside(Vec3f minCorner, Vec3f maxCorner) const;
		};
	public:
		Camera() : Camera(Vec3f(0)) {}
		Camera(Vec3f pos)
		{
			this->pos = pos;
			front = Vec3f(0.0f, 0.0f, -1.0f);
			up = Vec3f(0.0f, 1.0f, 0.0f);
		}

		void	update(float delta, float aspectRatio);
		void	imgui(void);

		Mat4f	getViewMatrix()
		{
			return (lookAt<float>(Vec3f(0.0), Vec3f(0.0) + front, up));
		}
		Mat4f	getProjectionMatrix()
		{
			return (perspective<float>(fov, _aspect_ratio, 0.1, 1000.0));
		}

		double	yaw = 0;
		double	pitch = 0;
		Vec3d	pos;
		Vec3d	front;
		Vec3d	up;
		Frustum	frustum;
		float	speed = 0;

		int		fov = 90;
	private:
		float	_aspect_ratio;
		void	_updatePlaneNormals();
		Vec3f	_direction;
		Vec3f	_lastPos;
};

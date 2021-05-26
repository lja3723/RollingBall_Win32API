#pragma once
#ifndef __physics_h__
#define __physics_h__
#include "scaler.h"

typedef double g_val;
typedef double kg_val;

namespace RollingBall
{
	class PixelVector
	{
	public:
		pixel x;
		pixel y;
	};

	class PhysicalVector
	{
	public:
		cm_val x;
		cm_val y;
	};

	class PhysicalValue
	{
	public:
		g_val mass;
		pixel size;
		PixelVector pos, speed, accel;
		double rotate_angle;
	};
}

#endif
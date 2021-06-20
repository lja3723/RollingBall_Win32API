#pragma once
#ifndef __physics_h__
#define __physics_h__

typedef double g_val;
typedef double kg_val;
typedef double cm_val;
typedef double m_val;

namespace RollingBall
{
	template <typename Length>
	class BasicPhysicalVector
	{
	public:
		Length x;
		Length y;

		BasicPhysicalVector(Length _x = 0, Length _y = 0) { x = _x, y = _y; }
		BasicPhysicalVector<Length>& operator()(Length _x, Length _y) { x = _x, y = _y; return *this; }
		BasicPhysicalVector<Length>& operator+=(BasicPhysicalVector<Length>& v);
		BasicPhysicalVector<Length>& operator-=(BasicPhysicalVector<Length>& v);
		int operator==(BasicPhysicalVector<Length>& v);
		int operator!=(BasicPhysicalVector<Length>& v);
	};

	template <typename Length, typename Mass>
	class NormalPhysicalValue
	{
	public:
		Mass mass;
		Length size;
		BasicPhysicalVector<Length> pos, speed, accel;
		double rotate_angle;
	};

	typedef NormalPhysicalValue<cm_val, g_val> PhysicalValue;
	typedef BasicPhysicalVector<cm_val> PhysicalVector;
	
}

#endif
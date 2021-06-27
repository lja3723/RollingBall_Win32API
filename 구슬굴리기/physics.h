#pragma once
#ifndef __physics_h__
#define __physics_h__
#include <cmath>

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
		BasicPhysicalVector<Length>& operator()(Length _x = 0, Length _y = 0) { x = _x, y = _y; return *this; }
		BasicPhysicalVector<Length>& operator()(const BasicPhysicalVector<Length>& v) { return operator()(v.x, v.y); }
		BasicPhysicalVector<Length>& operator+=(BasicPhysicalVector<Length>& v);
		BasicPhysicalVector<Length>& operator-=(BasicPhysicalVector<Length>& v);
		BasicPhysicalVector<Length> operator+(const BasicPhysicalVector<Length>& v) {
			return BasicPhysicalVector<Length>(x + v.x, y + v.y);
		}
		BasicPhysicalVector<Length> operator-(const BasicPhysicalVector<Length>& v) {
			return BasicPhysicalVector<Length>(x - v.x, y - v.y);
		}
		int operator==(BasicPhysicalVector<Length>& v)
		{
			return x == v.x && y == v.y;
		}
		int operator!=(BasicPhysicalVector<Length>& v) {
			return !operator==(v);
		}
		Length distance(const BasicPhysicalVector<Length>& v) {
			return sqrt(pow((*this - v).x, 2) + pow((*this - v).y, 2));
		}

	};

	template <typename Length, typename Mass>
	class NormalPhysicalValue
	{
	public:
		Mass mass;
		Length size;
		BasicPhysicalVector<Length> pos, speed, accel;
		double rotate_angle;
		
		NormalPhysicalValue(Mass _mass = 1, Length _size = 1, double _rotate_angle = 0) 
			: mass(_mass), size(_size), rotate_angle(_rotate_angle) {}
	};

	typedef NormalPhysicalValue<cm_val, g_val> PhysicalValue;
	typedef BasicPhysicalVector<cm_val> PhysicalVector;
}

#endif
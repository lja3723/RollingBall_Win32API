#include "physics.h"

using namespace RollingBall;

template<typename Length>
BasicPhysicalVector<Length>& BasicPhysicalVector<Length>::operator+=(BasicPhysicalVector& v)
{
	x += v.x;
	y += v.y;
	return this;
}
template<typename Length>
BasicPhysicalVector<Length>& BasicPhysicalVector<Length>::operator-=(BasicPhysicalVector& v)
{
	x -= v.x;
	y -= v.y;
	return this;
}
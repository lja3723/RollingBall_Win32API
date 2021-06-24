#include "scaler.h"

using namespace RollingBall;

Scaler::Scaler(double m_px_rate)
{
	px_rate(m_px_rate);
	_fix_point_physical(0, 0);
	_fix_point_pixel(0, 0);
}

void Scaler::px_rate(double px)
{
	_px_rate = px;
}
double Scaler::px_rate()
{
	return _px_rate;
}

pixel Scaler::px(cm_val cm)
{
	return int(cm * _px_rate + 0.5); //반올림을 위함임
}
cm_val Scaler::cm(pixel px)
{
	return (cm_val)px / _px_rate;
}

void Scaler::set_fix_point(PhysicalVector& point)
{
	_fix_point_physical = point;
}
void Scaler::set_fix_point(PhysicalVector&& point)
{
	set_fix_point(point);
}
PhysicalVector& Scaler::get_fix_point_physical()
{
	return _fix_point_physical;
}

void Scaler::set_fix_point(PixelCoord& point)
{
	_fix_point_pixel = point;
}
void Scaler::set_fix_point(PixelCoord&& point)
{
	set_fix_point(point);
}
PixelCoord& Scaler::get_fix_point_pixel()
{
	return _fix_point_pixel;
}

PhysicalVector Scaler::transform(PixelCoord& point)
{
	PhysicalVector result = _fix_point_physical;

	result.x += cm(point.x - _fix_point_pixel.x);
	result.y -= cm(point.y - _fix_point_pixel.y);

	return result;
}
PhysicalVector Scaler::transform(PixelCoord&& point)
{
	return transform(point);
}
PixelCoord Scaler::transform(PhysicalVector& point)
{
	PixelCoord result = _fix_point_pixel;

	result.x += px(point.x - _fix_point_physical.x);
	result.y -= px(point.y - _fix_point_physical.y);

	return result;
}
PixelCoord Scaler::transform(PhysicalVector&& point)
{
	return transform(point);
}

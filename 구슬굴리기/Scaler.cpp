#include "Scaler.h"

using namespace RollingBall;

int Scaler::scale_px_per_cm = 30;

void Scaler::set_scale_px_per_cm(int px)
{
	scale_px_per_cm = px;
}

int Scaler::px(double cm)
{
	return int(cm * (double)scale_px_per_cm + 0.5); //반올림을 위함임
}

double Scaler::cm(int px)
{
	return (double)px / scale_px_per_cm;
}

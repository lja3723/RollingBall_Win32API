#include "scaler.h"

using namespace RollingBall;

pixel Scaler::scale_px_per_cm = 30;

void Scaler::set_scale_px_per_cm(pixel px)
{
	scale_px_per_cm = px;
}

pixel Scaler::px(cm_val cm)
{
	return int(cm * (cm_val)scale_px_per_cm + 0.5); //반올림을 위함임
}

cm_val Scaler::cm(pixel px)
{
	return (cm_val)px / scale_px_per_cm;
}

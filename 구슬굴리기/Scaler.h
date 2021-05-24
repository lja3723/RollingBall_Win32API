#pragma once
#ifndef __Scaler_h__
#define __Scaler_h__

namespace RollingBall
{

	class Scaler {
	private:
		static int scale_px_per_cm;
	public:
		void set_scale_px_per_cm(int px);
		int px(double cm);
		double cm(int px);
	};

}

#endif
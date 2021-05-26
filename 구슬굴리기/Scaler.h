#pragma once
#ifndef __scaler_h__
#define __scaler_h__

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
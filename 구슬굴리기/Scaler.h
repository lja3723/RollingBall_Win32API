#pragma once
#ifndef __scaler_h__
#define __scaler_h__

namespace RollingBall
{

	typedef int pixel;
	typedef double cm_val;
	typedef double m_val;

	class Scaler {
	private:
		static pixel scale_px_per_cm;
	public:
		void set_scale_px_per_cm(pixel px);
		pixel px(cm_val cm);
		cm_val cm(pixel px);
	};

}

#endif
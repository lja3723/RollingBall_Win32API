#ifndef __device_h__
#define __device_h__

#include "device_input.h"
#include "device_output.h"

namespace RollingBall
{
	class Device
	{
	public:
		OutputDevice out;
		InputDevice in;

	public:
		void init();
	};
}


#endif
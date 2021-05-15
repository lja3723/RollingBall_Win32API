#pragma once
#ifndef __BitmapManage_h__
#define __BitmapManage_h__
#include <Windows.h>
#include "resource.h"

/*
* 
* BitmapManage:
* ���α׷����� ����ϴ� ��Ʈ���� �����ϴ� Ŭ������ ����
* 
*/


namespace RollingBall
{
	//���� ��Ʈ�� �ؽ��� ũ�� ������ ����Ѵ�.
	const static int BallSizeTypes = 4;

	//���� ��Ʈ�� �ؽ��� ũ�� ������ �� ũ�⸦ ����Ѵ�.
	const static int BallSize_small = 32;
	const static int BallSize_medium = 64;
	const static int BallSize_large = 128;
	const static int BallSize_extra = 256;

	//���� �׸��� ��Ʈ���� �� ������ ũ�⸦ ����Ѵ�.
	const static int BallSize_small_shadow = 36;
	const static int BallSize_medium_shadow = 72;
	const static int BallSize_large_shadow = 143;
	const static int BallSize_extra_shadow = 286;

	//�ٴ� �ؽ��� ũ�⸦ ����Ѵ�.
	const static int FloorSize = 256;

	class BitmapManager
	{
	public:
	private:
		HBITMAP floor;
		HBITMAP ball[BallSizeTypes];
		HBITMAP ball_mask[BallSizeTypes];
		HBITMAP ball_shadow[BallSizeTypes];
		HINSTANCE hInstance;

	public:
		void initialize(HINSTANCE m_hInstance);

		HBITMAP get_hBitmap_floor();
		HBITMAP get_hBitmap_ball(int BallSize);
		HBITMAP get_hBitmap_ball_mask(int BallSize);
		HBITMAP get_hBitmap_ball_shadow(int BallSize);
	private:
		int BallSize_toIdx(int BallSize);
	};


}

#endif
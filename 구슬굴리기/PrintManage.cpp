#include "PrintManage.h"

void RollingBall::PrintManager::print_background_ruller()
{
}

void RollingBall::PrintManager::initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType)
{
	hInstance = m_hInstance;
	hwnd = m_hwnd;
	BallSizeType = m_BallSizeType;
	bmpManager.initialize(hInstance);
}

void RollingBall::PrintManager::set_BallSizeType(int BallSize)
{
	BallSizeType = BallSizeType;
}

void RollingBall::PrintManager::print_background(LPRECT region)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HBITMAP oldBit, hBackground;
	hdc = BeginPaint(hwnd, &ps);
	//hdc = GetDC(hwnd);

	{
		oldBit, hBackground = bmpManager.get_hBitmap_floor();
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBackground);

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 4; j++)
				BitBlt(hdc, i * 256, j * 256, 256, 256, memdc, 0, 0, SRCCOPY);
	}

	SelectObject(memdc, oldBit);
	DeleteDC(memdc);
	EndPaint(hwnd, &ps);
	//ReleaseDC(hwnd, hdc);
}

void RollingBall::PrintManager::print_ball(int x, int y)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	//hdc = BeginPaint(hwnd, &ps);
	hdc = GetDC(hwnd);

	HBITMAP oldBit;
	memdc = CreateCompatibleDC(hdc);

	{
		HBITMAP hMask = bmpManager.get_hBitmap_ball_mask(BallSizeType);
		HBITMAP hBall = bmpManager.get_hBitmap_ball(BallSizeType);
		oldBit = (HBITMAP)SelectObject(memdc, hMask);
		BitBlt(
			hdc,
			x - BallSizeType / 2, y - BallSizeType / 2,
			BallSizeType, BallSizeType,
			memdc, 0, 0, SRCAND
		);

		SelectObject(memdc, hBall);
		BitBlt(
			hdc,
			x - BallSizeType / 2, y - BallSizeType / 2,
			BallSizeType, BallSizeType,
			memdc, 0, 0, SRCPAINT
		);

	}

	SelectObject(memdc, oldBit);
	DeleteDC(memdc);
	//EndPaint(hwnd, &ps);
	ReleaseDC(hwnd, hdc);
}

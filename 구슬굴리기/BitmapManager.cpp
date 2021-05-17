#include "BitmapManager.h"

using namespace RollingBall;

int BitmapManager::object_count = 0;
HINSTANCE BitmapManager::hInstance = NULL;
HWND BitmapManager::hwnd = NULL;
HBITMAP BitmapManager::floor = NULL;
HBITMAP BitmapManager::ball[BallSizeCount] = { NULL, };
HBITMAP BitmapManager::ball_mask[BallSizeCount] = { NULL, };

BitmapManager::BitmapManager()
{
	hDCwindowBuffer = NULL;
	set_BallSizeType(BallSize_medium);
}
BitmapManager::~BitmapManager()
{
	if (object_count == 0)
	{
		hInstance = NULL;
		DeleteObject(floor);
		for (int size = 0; size < BallSizeCount; size++)
		{
			DeleteObject(ball[size]);
			DeleteObject(ball_mask[size]);
		}
	}
	release_hDCwindowBuffer();
	object_count--;
}


void BitmapManager::initialize(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType)
{
	if (object_count == 0)
	{
		//ºñÆ®¸Ê ·Îµå
		hInstance = m_hInstance;
		hwnd = m_hwnd;
		floor = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLOOR));
		for (int size = 0; size < BallSizeCount; size++)
		{
			ball[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BALL_032 + 2 * size));
			ball_mask[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP_BALL_032_MASK + 2 * size));
		}
	}
	set_BallSizeType(m_BallSizeType);
	set_hDCwindowBuffer();

	object_count++;
}
BOOL BitmapManager::isInitialized()
{
	return object_count > 0;
}


void BitmapManager::set_BallSizeType(int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
}
void BitmapManager::set_hDCwindowBuffer()
{
	if (hDCwindowBuffer != NULL) 
		release_hDCwindowBuffer();
	RECT rt;
	HDC hdc = GetDC(hwnd);

	GetClientRect(hwnd, &rt);
	hDCwindowBuffer = CreateCompatibleBitmap(hdc, rt.right, rt.left);
	ReleaseDC(hwnd, hdc);
}


HBITMAP BitmapManager::get_hBitmap_floor()
{
	return floor;
}
HBITMAP BitmapManager::get_hBitmap_ball()
{
	return ball[BallSize_toIdx(BallSizeType)];
}
HBITMAP BitmapManager::get_hBitmap_ball_mask()
{
	return ball_mask[BallSize_toIdx(BallSizeType)];
}
HBITMAP BitmapManager::get_hBitmap_hDCwindowBuffer()
{
	return hDCwindowBuffer;
}
int BitmapManager::get_BallSizeType()
{
	return BallSizeType;
}


void BitmapManager::release_hDCwindowBuffer()
{
	if (hDCwindowBuffer == NULL) return;

	DeleteObject(hDCwindowBuffer);
	hDCwindowBuffer = NULL;
}


int BitmapManager::BallSize_toIdx(int BallSize)
{
	switch (BallSize)
	{
	case BallSize_small:
		return 0;
	case BallSize_medium:
		return 1;
	case BallSize_large:
		return 2;
	case BallSize_extra:
		return 3;
	default:
		return -1;
	}
}

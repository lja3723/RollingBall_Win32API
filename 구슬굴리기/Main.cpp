#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "BitmapManage.h"
#include "PhysicsControl.h"
#include "PrintManage.h"

using namespace RollingBall;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
static LPCTSTR WindowClassName = _T("Rolling Ball Class");
static LPCTSTR WindowTitleName = _T("구슬 굴리기");
static const int WindowPosition[2] = { CW_USEDEFAULT, CW_USEDEFAULT };
static const int WindowSize[2] = { 720, 480 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	WndClass.lpszClassName = WindowClassName;

	RegisterClass(&WndClass);
	hwnd = CreateWindow(WindowClassName, WindowTitleName,
		WS_OVERLAPPEDWINDOW,
		WindowPosition[0], WindowPosition[1],
		WindowSize[0], WindowSize[1],
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hBit, oldBit;
	HDC hdc, memdc;
	PAINTSTRUCT ps;

	static BitmapManager bmpManager;

	switch (iMsg) {
	case WM_CREATE:
		bmpManager.initialize(((LPCREATESTRUCT)lParam)->hInstance);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBit = bmpManager.get_hBitmap_floor();
		memdc = CreateCompatibleDC(hdc);
		
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 6; j++)
				BitBlt(hdc, i*256, j*256, 256, 256, memdc, 0, 0, SRCCOPY);

		//BallSize_small 출력
		{
			hBit = bmpManager.get_hBitmap_ball_mask(BallSize_small);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 50, 100, 256, 256, memdc, 0, 0, SRCAND);

			hBit = bmpManager.get_hBitmap_ball(BallSize_small);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 50, 100, 256, 256, memdc, 0, 0, SRCPAINT);
		}

		//BallSize_medium 출력
		{
			hBit = bmpManager.get_hBitmap_ball_mask(BallSize_medium);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 100, 100, 256, 256, memdc, 0, 0, SRCAND);

			hBit = bmpManager.get_hBitmap_ball(BallSize_medium);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 100, 100, 256, 256, memdc, 0, 0, SRCPAINT);
		}

		//BallSize_large 출력
		{
			hBit = bmpManager.get_hBitmap_ball_mask(BallSize_large);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 180, 100, 256, 256, memdc, 0, 0, SRCAND);

			hBit = bmpManager.get_hBitmap_ball(BallSize_large);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 180, 100, 256, 256, memdc, 0, 0, SRCPAINT);
		}

		//BallSize_extra 출력
		{
			hBit = bmpManager.get_hBitmap_ball_mask(BallSize_extra);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 290, 120, 256, 256, memdc, 0, 0, SRCAND);

			hBit = bmpManager.get_hBitmap_ball(BallSize_extra);
			SelectObject(memdc, hBit);
			BitBlt(hdc, 290, 120, 256, 256, memdc, 0, 0, SRCPAINT);
		}

		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

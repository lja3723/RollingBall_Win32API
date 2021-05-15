#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "BitmapManage.h"
#include "PhysicsControl.h"
#include "PrintManage.h"

using namespace RollingBall;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
static LPCTSTR WindowClassName = _T("Rolling Ball Class");
static LPCTSTR WindowTitleName = _T("±¸½½ ±¼¸®±â");
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
	static PrintManager printManager;

	static int x, y;

	static bool pushed[4] = { false, false, false, false };

	switch (iMsg) {
	case WM_CREATE:
		printManager.initialize(((LPCREATESTRUCT)lParam)->hInstance, hwnd);
		SetTimer(hwnd, 1, 5, NULL);
		return 0;
	case WM_PAINT:
		printManager.print_background(NULL);
		printManager.print_ball(x, y);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			pushed[0] = true;
			break;
		case VK_RIGHT:
			pushed[1] = true;
			break;
		case VK_UP:
			pushed[2] = true;
			break;
		case VK_DOWN:
			pushed[3] = true;
			break;
		}
		return 0;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			pushed[0] = false;
			break;
		case VK_RIGHT:
			pushed[1] = false;
			break;
		case VK_UP:
			pushed[2] = false;
			break;
		case VK_DOWN:
			pushed[3] = false;
			break;
		}
		return 0;
	case WM_TIMER:
		if (pushed[0]) x -= 5;
		if (pushed[1]) x += 5;
		if (pushed[2]) y -= 5;
		if (pushed[3]) y += 5;
		InvalidateRgn(hwnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "RollingBall.h"

using namespace RollingBall;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DebuggingDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

static LPCTSTR WindowClassName = _T("Rolling Ball Class");
static LPCTSTR WindowTitleName = _T("���� ������");
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

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateAccelerator(hwnd, hAccel, &msg);
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	static RollingBallClass rollingBall;
	static HWND hDlg = NULL;

	switch (iMsg) 
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 5, NULL);
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		rollingBall.initialize(hInstance, hwnd);
		return 0;

	case WM_TIMER:
		rollingBall.update_state();
		InvalidateRgn(hwnd, NULL, FALSE);
		return 0;

	case WM_PAINT:
		rollingBall.update_window();
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		rollingBall.send_windowEvent(iMsg, wParam, lParam);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_DEBUGGING:
			if (!IsWindow(hDlg))
			{
				hDlg = CreateDialog(
					hInstance, MAKEINTRESOURCE(IDD_DIALOG_DEBUGGING), 
					hwnd, (DLGPROC)DebuggingDialogProc
				);
				ShowWindow(hDlg, SW_SHOW);
			}
			break;
		}
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DebuggingDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hDlg);
			hDlg = NULL;
			break;
		}
		return TRUE;
	}

	return FALSE;
}
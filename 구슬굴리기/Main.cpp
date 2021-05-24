#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "rolling_ball.h"
#include "debugger.h"
#define PROGRAM_NAME _T("구슬 굴리기")
#define PROGRAM_VER	_T("1.1")

using namespace RollingBall;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DebuggingDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ProgramInfoDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

static LPCTSTR WindowClassName = _T("Rolling Ball Class");
static LPCTSTR WindowTitleName = PROGRAM_NAME;
static const int WindowPosition[2] = { CW_USEDEFAULT, CW_USEDEFAULT };
static const int WindowSize[2] = { CW_USEDEFAULT, CW_USEDEFAULT };
static RollingBallClass rollingBall;

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

	//RollingBallClass를 초기화한다
	if (!rollingBall.init(hInstance, hwnd))
	{
		MessageBox(hwnd, _T("프로그램을 시작할 수 없습니다."), _T("오류"), MB_OK);
		return 0;
	}

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
	static HWND hDebugDlg = NULL;
	static HWND hProgramInfoDlg = NULL;

	switch (iMsg) 
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 5, NULL);
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		debugger.init(hInstance, hwnd);
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
	case WM_SIZE:
		rollingBall.send_windowEvent(iMsg, wParam, lParam);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_DEBUGGING:
			if (!IsWindow(hDebugDlg))
			{
				hDebugDlg = CreateDialog(
					hInstance, MAKEINTRESOURCE(IDD_DIALOG_DEBUGGING), 
					hwnd, (DLGPROC)DebuggingDialogProc
				);
				debugger.set_hDebugDlg(hDebugDlg);
				ShowWindow(hDebugDlg, SW_SHOW);
			}
			break;
		case ID_PROGRAM_INFO:
			if (!IsWindow(hProgramInfoDlg))
			{
				hProgramInfoDlg = CreateDialog(
					hInstance, MAKEINTRESOURCE(IDD_DIALOG_PROGRAM_INFO),
					hwnd, (DLGPROC)ProgramInfoDialogProc
				);
				SetDlgItemText(hProgramInfoDlg, IDC_TEXT_PROGRAM_NAME, PROGRAM_NAME);
				SetDlgItemText(hProgramInfoDlg, IDC_TEXT_PROGRAM_VER, PROGRAM_VER);
				ShowWindow(hProgramInfoDlg, SW_SHOW);
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
			debugger.release_hDebugDlg();
			break;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CALLBACK ProgramInfoDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
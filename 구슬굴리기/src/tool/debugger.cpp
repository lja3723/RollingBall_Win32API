#include "debugger.h"

using namespace RollingBall;



/*****************************
*	static variables
******************************/
BOOL Debugger::isCmdShow = FALSE;
HINSTANCE Debugger::hInstance = NULL;
HWND Debugger::hwnd = NULL;
TCHAR Debugger::buff[1024] = { 0, };



/*****************************
*	public functions
******************************/
void Debugger::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	hInstance = m_hInstance;
	hwnd = m_hwnd;
}

void RollingBall::Debugger::showCmd()
{
	if (isCmdShow) return;

	if (AllocConsole())
	{
		//SetConsoleTitle(_T("Debugging Console"));

		isCmdShow = TRUE;
	}
}

void RollingBall::Debugger::hideCmd()
{
	if (!isCmdShow) return;

	if (FreeConsole)
		isCmdShow = FALSE;
}

void Debugger::clearBuff()
{
	memset(buff, 0, sizeof(buff));
}
void Debugger::printBuff()
{
	MessageBox(hwnd, buff, _T("Debugger Message"), MB_OK);
}


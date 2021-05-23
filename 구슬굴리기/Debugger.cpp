#include "Debugger.h"

using namespace RollingBall;



/*****************************
*	static variables
******************************/
HINSTANCE Debugger::hInstance = NULL;
HWND Debugger::hwnd = NULL;
HWND Debugger::hDebugDlg = NULL;



/*****************************
*	public functions
******************************/
void Debugger::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	hInstance = m_hInstance;
	hwnd = m_hwnd;
}

BOOL Debugger::isInit()
{
	return hInstance != NULL;
}
BOOL Debugger::isSetHDebugDlg()
{
	return hDebugDlg != NULL;
}
void Debugger::set_hDebugDlg(HWND m_hDebugDlg)
{
	hDebugDlg = m_hDebugDlg;
}
void Debugger::set_hDebugDlgText(int text_num, LPCTSTR text)
{
	if (!isSetHDebugDlg()) return;
	SetDlgItemText(hDebugDlg, IDC_TEXT_DEBUG1 + text_num - 1, text);
}
void Debugger::release_hDebugDlg()
{
	hDebugDlg = NULL;
}

void Debugger::clearBuff()
{
	memset(buff, 0, sizeof(buff));
}
void Debugger::printBuff()
{
	MessageBox(hwnd, buff, _T("Debugger Message"), MB_OK);
}


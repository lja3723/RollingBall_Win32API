#pragma once
#ifndef __debugger_h__
#define __debugger_h__

#include <Windows.h>
#include <tchar.h>
#include "../../resource.h"

#define debuggerMessage(Format, ...) \
	debugger.clearBuff(), \
	_stprintf_s(debugger.buff, debugger.szbuff, _T(Format), __VA_ARGS__), \
	debugger.printBuff()

#define debuggerBuffSet(Format, ...) \
	debugger.clearBuff(), \
	_stprintf_s(debugger.buff, debugger.szbuff, _T(Format), __VA_ARGS__)

namespace RollingBall {

	class Debugger {
	private:
		static HINSTANCE hInstance;
		static HWND hwnd;
		static HWND hDebugDlg;

	public:
		TCHAR buff[1024];
		const static int szbuff = sizeof(buff) / sizeof(TCHAR);

		void init(HINSTANCE m_hInstance, HWND m_hwnd);
		BOOL isInit();
		BOOL isSetHDebugDlg();
		void set_hDebugDlg(HWND m_hDebugDlg);
		void set_hDebugDlgText(int text_num, LPCTSTR text);
		void release_hDebugDlg();

		void clearBuff();
		void printBuff();

	};

	static Debugger debugger;
}


#endif
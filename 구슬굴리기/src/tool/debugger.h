#pragma once
#ifndef __debugger_h__
#define __debugger_h__

#include <Windows.h>
#include <tchar.h>
#include "../../resource.h"


#define debuggerBuffSet(Format, ...) \
	debugger.clearBuff(), \
	_stprintf_s(debugger.buff, debugger.szbuff, _T(Format), __VA_ARGS__)

#define debuggerMessage(Format, ...) \
	debuggerBuffSet(Format, __VA_ARGS__) \
	debugger.printBuff()

namespace RollingBall {

	class Debugger {
	private:
		static BOOL isCmdShow;
		static HINSTANCE hInstance;
		static HWND hwnd;
		static TCHAR buff[1024];
		const static int szbuff = sizeof(buff) / sizeof(TCHAR);
	public:
		Debugger() = delete;

		static void init(HINSTANCE m_hInstance, HWND m_hwnd);
		static void showCmd();
		static void hideCmd();

		void clearBuff();
		void printBuff();

	};
}


#endif
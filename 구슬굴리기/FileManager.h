#pragma once
#ifndef __FileManager_h__
#define __FileManager_h__

#include <Windows.h>
#include <tchar.h>

namespace RollingBall 
{
	class FileManager
	{
	private:
		HANDLE hFile;

	public:
		BOOL isOpen();
		FileManager() { hFile = NULL; };
		BOOL open(LPCTSTR filename, LPCTSTR mode);
		void close();
		BOOL readLine(LPTSTR line, int sizeofLine);
	};
}

#endif
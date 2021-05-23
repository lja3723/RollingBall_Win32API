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
		FileManager() { hFile = NULL; };

		//파일이 열렸는지 검사하는 함수
		BOOL isOpen();

		//파일을 여는 함수
		BOOL open(LPCTSTR filename, LPCTSTR mode);

		//파일을 닫는 함수
		void close();

		//파일에서 한 라인씩 읽어서 line에 저장해주는 함수
		BOOL readLine(LPTSTR line, int sizeofLine);

		BOOL readFile(
			LPVOID lpBuffer,
			DWORD nNumberOfBytesToRead,
			LPDWORD lpNumberOfBytesRead,
			LPOVERLAPPED lpOvrelapped
		);

		BOOL writeFile(
			LPVOID lpBuffer,
			DWORD nNumberOfBytesToWrite,
			LPDWORD lpNumberOfBytesWritten,
			LPOVERLAPPED lpOvrelapped
		);
	};
}

#endif
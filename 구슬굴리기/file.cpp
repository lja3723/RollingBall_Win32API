#include "file.h"

using namespace RollingBall;

BOOL File::isOpen()
{
	return hFile != NULL;
}

BOOL File::open(LPCTSTR filename, LPCTSTR mode)
{
	BOOL GENERIC_FLAG, FILE_SHARE_FLAG;

	//mode 해석
	if (_tcscmp(mode, _T("r")) == 0 || _tcscmp(mode, _T("R")) == 0)
	{
		GENERIC_FLAG = GENERIC_READ;
		FILE_SHARE_FLAG = FILE_SHARE_READ;
	}
	else if (_tcscmp(mode, _T("w")) == 0 || _tcscmp(mode, _T("W")) == 0)
	{
		GENERIC_FLAG = GENERIC_WRITE;
		FILE_SHARE_FLAG = FILE_SHARE_WRITE;
	}
	else if (
		_tcscmp(mode, _T("rw")) == 0 || _tcscmp(mode, _T("rW")) == 0 ||
		_tcscmp(mode, _T("Rw")) == 0 || _tcscmp(mode, _T("RW")) == 0 ||
		_tcscmp(mode, _T("wr")) == 0 || _tcscmp(mode, _T("Wr")) == 0 ||
		_tcscmp(mode, _T("wR")) == 0 || _tcscmp(mode, _T("WR")) == 0
		)
	{
		GENERIC_FLAG = GENERIC_READ | GENERIC_WRITE;
		FILE_SHARE_FLAG = FILE_SHARE_READ | FILE_SHARE_WRITE;
	}
	else return FALSE;

	//파일 열기
	hFile = CreateFile(
		filename,
		GENERIC_FLAG,
		FILE_SHARE_FLAG,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL
	);

	//파일 열기 실패시
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

void File::close()
{
	if (!isOpen()) return;
	CloseHandle(hFile);
	hFile = NULL;
}

BOOL File::readLine(LPTSTR line, int sizeofLine)
{
	if (!isOpen()) return FALSE;

	//파일의 끝에 다다르면 FALSE를 리턴한다.
	DWORD readSize;
	TCHAR reading[1];
	static int i = -1;
	int lineidx = 0;
	memset(line, 0, sizeofLine);
	while (TRUE)
	{
		memset(reading, 0, sizeof(reading));
		ReadFile(hFile, reading, sizeof(reading), &readSize, NULL);
		if (++i == 0) continue;

		if (reading[0] == _T('\r'))
		{
			return TRUE;
		}
		if (reading[0] == _T('\n')) continue;
		if (reading[0] == _T('\0'))
		{
			if (lineidx > 0)
				return TRUE;
			else return FALSE;
		}

		line[lineidx++] = reading[0];
	}

	return 0;
}

BOOL File::readFile(
	LPVOID lpBuffer, DWORD nNumberOfBytesToRead, 
	LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOvrelapped)
{
	return ReadFile(
		hFile, lpBuffer, 
		nNumberOfBytesToRead, lpNumberOfBytesRead, lpOvrelapped
	);
}

BOOL File::writeFile(
	LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, 
	LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOvrelapped)
{
	return WriteFile(
		hFile, lpBuffer,
		nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOvrelapped
	);
}

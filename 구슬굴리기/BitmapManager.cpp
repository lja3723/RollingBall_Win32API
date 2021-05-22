#include "BitmapManager.h"

using namespace RollingBall;



/*****************************************
*
*	static variables initialization
* 
*****************************************/
UINT BitmapManager::bitmap_file_count = 0;
HINSTANCE BitmapManager::hInstance = NULL;
HBITMAP* BitmapManager::hBitmap = NULL;
//int BitmapManager::_object_count = 0;


//old variables below
HBITMAP BitmapManager::oldvar_floor = NULL;
HBITMAP BitmapManager::oldvar_ball[old_BallSizeCount] = { NULL, };
HBITMAP BitmapManager::oldvar_ball_mask[old_BallSizeCount] = { NULL, };



/*****************************************
*
*			private functions
* 
*****************************************/
BOOL RollingBall::BitmapManager::init_object_info(HWND hwnd)
{
	//파일을 연다.
	LPCTSTR filename = _T("..\\res\\bmp\\object_info.txt");
	HANDLE hFile = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL
	);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TCHAR errmsg[256];
		_stprintf_s(errmsg, 256, _T("%s 파일을 열 수 없습니다."), filename);
		MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
		return FALSE;
	}

	//파일을 한 줄씩 읽어 object에 저장한다.
	TCHAR line[128];
	BOOL isObjectReading = FALSE;
	int objidx = -1;
	while (ReadLine(hFile, line, sizeof(line)))
	{
		//공백 라인이거나 주석일 경우 패스
		if (_tcslen(line) == 0) continue;
		if (line[0] == _T('#')) continue;

		//오브젝트 정보를 읽는 중이 아닐 때...
		if (!isObjectReading) {

			//오브젝트 개수를 설정함
			if (_tcscmp(line, _T("object_count=")) == 0) {
				ReadLine(hFile, line, sizeof(line));
				object_info.resize(_ttoi(line));
			}

			//오브젝트 정보를 읽기 시작함
			if (_tcscmp(line, _T("<object>")) == 0) {
				objidx++;
				isObjectReading = TRUE;
				continue;
			}
		}

		//오브젝트 정보를 읽는 중일 때..
		if (isObjectReading)
		{
			//오브젝트의 이름을 저장
			if (_tcscmp(line, _T("name=")) == 0) {
				ReadLine(hFile, line, sizeof(line));
				object_info[objidx].name = line;
				continue;
			}

			//오브젝트 텍스쳐 개수 설정
			if (_tcscmp(line, _T("texture_count=")) == 0) {
				ReadLine(hFile, line, sizeof(line));
				object_info[objidx].texture.name.resize(_ttoi(line));
				continue;
			}

			//오브젝트 텍스쳐 이름 저장
			if (_tcscmp(line, _T("texture_name=")) == 0) {
				for (int i = 0; i < object_info[objidx].texture.name.size(); i++) {
					ReadLine(hFile, line, sizeof(line));
					object_info[objidx].texture.name[i] = line;
				}
				continue;
			}

			//오브젝트 텍스쳐 크기 값 개수 설정
			if (_tcscmp(line, _T("texture_size_count=")) == 0) {
				ReadLine(hFile, line, sizeof(line));
				object_info[objidx].texture.value.resize(_ttoi(line));
				continue;
			}

			//오브젝트 텍스쳐 크기 값 저장
			if (_tcscmp(line, _T("texture_size_value=")) == 0) {
				for (int i = 0; i < object_info[objidx].texture.value.size(); i++) {
					ReadLine(hFile, line, sizeof(line));
					object_info[objidx].texture.value[i] = _ttoi(line);
				}
				continue;
			}

			//오브젝트의 마스크 여부 저장
			if (_tcscmp(line, _T("has_mask=")) == 0) {
				ReadLine(hFile, line, sizeof(line));
				if (_tcscmp(line, _T("TRUE")) == 0)
					object_info[objidx].has_mask = TRUE;
				if (_tcscmp(line, _T("FALSE")) == 0)
					object_info[objidx].has_mask = FALSE;
				continue;
			}
		}

		//오브젝트 정보를 모두 읽었음
		if (_tcscmp(line, _T("</object>")) == 0) {
			isObjectReading = FALSE;
			continue;
		}

		//파일이 종료됨
		if (_tcscmp(line, _T("</object_info.txt>")) == 0)
			break;
	}

	//파일을 닫는다.
	CloseHandle(hFile);
	return TRUE;
}
BOOL RollingBall::BitmapManager::ReadLine(HANDLE hFile, LPTSTR line, int sizeofLine)
{
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
}
void RollingBall::BitmapManager::init_bitmap_file_count()
{
	bitmap_file_count = 0;
	UINT object_files;
	for (int object = 0; object < old_object_count; object++)
	{
		object_files =
			old_count_table[object][old__texture] *
			old_count_table[object][old__size] *
			(old_count_table[object][old__mask] ? 2 : 1);
		bitmap_file_count += object_files;
	}
}
void RollingBall::BitmapManager::init_hBitmap()
{
	hBitmap = new HBITMAP[bitmap_file_count];

	for (UINT file_idx = 0; file_idx < bitmap_file_count; file_idx++)
		hBitmap[file_idx] = LoadBitmap(hInstance, MAKEINTRESOURCE(BMPFILEMACRO[file_idx]));

	//old code below
	//비트맵 로드
	oldvar_floor = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_FLOOR_WOOD1_256));
	for (int size = 0; size < old_BallSizeCount; size++)
	{
		oldvar_ball[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BALL_IRON1_032 + 2 * size));
		oldvar_ball_mask[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BALL_IRON1_032M + 2 * size));
	}
}
void RollingBall::BitmapManager::delete_hBitmap()
{
	for (UINT file_idx = 0; file_idx < bitmap_file_count; file_idx++)
		DeleteObject(hBitmap[file_idx]);

	delete[] hBitmap;

	//old code below
	DeleteObject(oldvar_floor);
	for (int size = 0; size < old_BallSizeCount; size++)
	{
		DeleteObject(oldvar_ball[size]);
		DeleteObject(oldvar_ball_mask[size]);
	}
}



/*****************************************
*
*			public functions
*
*****************************************/
BOOL BitmapManager::init(HINSTANCE m_hInstance, HWND m_hwnd, int m_BallSizeType)
{
	if (isInit()) return TRUE;
	if (!init_object_info(m_hwnd)) return FALSE;

	//debug code
	/*
	{
		TCHAR tmp[512];
		_stprintf_s(tmp, 512, _T("object count:%d"), _object.size());
		MessageBox(m_hwnd, tmp, _T(""), MB_OK);
		for (int i = 0; i < _object.size(); i++)
		{
			MessageBox(m_hwnd, _T("OBJECT"), _T(""), MB_OK);
			_stprintf_s(tmp, 512, _T("name:%s, has_mask:%d"),
				_object[i].name.c_str(), _object[i].has_mask);
			MessageBox(m_hwnd, tmp, _T(""), MB_OK);

			_stprintf_s(tmp, 512, _T("object texture count:%d"),
				_object[i].texture.name.size());
			MessageBox(m_hwnd, tmp, _T(""), MB_OK);
			for (int j = 0; j < _object[i].texture.name.size(); j++)
			{
				_stprintf_s(tmp, 512, _T("texture name(%d):%s"),
					j, _object[i].texture.name[j].c_str());
				MessageBox(m_hwnd, tmp, _T(""), MB_OK);
			}

			_stprintf_s(tmp, 512, _T("object texture size count:%d"),
				_object[i].texture.value.size());
			MessageBox(m_hwnd, tmp, _T(""), MB_OK);
			for (int j = 0; j < _object[i].texture.value.size(); j++)
			{
				_stprintf_s(tmp, 512, _T("texture size(%d):%d"),
					j, _object[i].texture.value[j]);
				MessageBox(m_hwnd, tmp, _T(""), MB_OK);
			}
		}
	}
	*/

	hInstance = m_hInstance;
	init_bitmap_file_count();
	init_hBitmap();
	old_set_BallSizeType(m_BallSizeType);

	return TRUE;
}
BOOL RollingBall::BitmapManager::isInit()
{
	return hInstance != NULL;
}
BitmapManager::~BitmapManager()
{
	if (isInit())
	{
		hInstance = NULL;
		delete_hBitmap();
	}
}

HBITMAP RollingBall::BitmapManager::get(int index)
{
	if (isInit()) return hBitmap[index];
	else return NULL;
}
int RollingBall::BitmapManager::index(int object, int texture, int size, BOOL mask)
{
	int idx = 0;

	//object에 따른 idx 탐색
	for (int cur_object = 0; cur_object < old_object_count; cur_object++)
	{
		if (cur_object == object) break;
		idx += 
			old_count_table[cur_object][old__texture] 
			* old_count_table[cur_object][old__size]
			* (old_count_table[cur_object][old__mask] ? 2 : 1);
	}

	//texture에 따른 idx 탐색
	for (UINT cur_texture = 0; cur_texture < old_count_table[object][old__texture]; cur_texture++)
	{
		if (cur_texture == texture) break;
		idx +=
			old_count_table[object][old__size] *
			(old_count_table[object][old__mask] ? 2 : 1);
	}

	//mask 유무에 따른 idx 탐색
	if (mask == TRUE && old_count_table[object][old__mask] == TRUE)
		idx += old_count_table[object][old__size];

	//size에 따른 idx 탐색
	idx += size;

	return idx;
}
int RollingBall::BitmapManager::object(LPCTSTR object)
{
	if (_tcscmp(object, _T("ball")) == 0)
		return 0;
	if (_tcscmp(object, _T("floor")) == 0)
		return 1;

	return 0;
}
int RollingBall::BitmapManager::texture(LPCTSTR texture)
{
	int result = 0;
	if (_tcscmp(texture, _T("iron1")) == 0)
		return 0;
	if (_tcscmp(texture, _T("iron2")) == 0)
		return 1;
	if (_tcscmp(texture, _T("wood1")) == 0)
		return 0;
	return 0;
}
int RollingBall::BitmapManager::size(int size)
{
	return 0;
}
HBITMAP RollingBall::BitmapManager::operator[](int index)
{
	return get(index);
}




//old functions below
void BitmapManager::old_set_BallSizeType(int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
}
int BitmapManager::old_get_BallSizeType()
{
	return BallSizeType;
}
HBITMAP BitmapManager::old_get_hBitmap_floor()
{
	return oldvar_floor;
}
HBITMAP BitmapManager::old_get_hBitmap_ball()
{
	return oldvar_ball[old_BallSize_toIdx(BallSizeType)];
}
HBITMAP BitmapManager::old_get_hBitmap_ball_mask()
{
	return oldvar_ball_mask[old_BallSize_toIdx(BallSizeType)];
}
int BitmapManager::old_BallSize_toIdx(int BallSize)
{
	switch (BallSize)
	{
	case old_BallSize_small:
		return 0;
	case old_BallSize_medium:
		return 1;
	case old_BallSize_large:
		return 2;
	case old_BallSize_extra:
		return 3;
	default:
		return -1;
	}
}
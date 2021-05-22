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


//old variables below
HBITMAP BitmapManager::oldvar_floor = NULL;
HBITMAP BitmapManager::oldvar_ball[BallSizeCount] = { NULL, };
HBITMAP BitmapManager::oldvar_ball_mask[BallSizeCount] = { NULL, };



/*****************************************
*
*			private functions
* 
*****************************************/
BOOL RollingBall::BitmapManager::init_object_info(HWND hwnd)
{
	LPCTSTR filename = _T("..\\res\\bmp\\object_info.txt");
	HANDLE hFile = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 0, 0
	);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TCHAR errmsg[256];
		_stprintf_s(errmsg, 256, _T("%s 파일을 열 수 없습니다."), filename);
		MessageBox(hwnd, errmsg, _T("오류"), MB_OK);
		return FALSE;
	}

	return TRUE;
}
void RollingBall::BitmapManager::init_bitmap_file_count()
{
	bitmap_file_count = 0;
	UINT object_files;
	for (int object = 0; object < object_count; object++)
	{
		object_files =
			count_table[object][_texture] *
			count_table[object][_size] *
			(count_table[object][_mask] ? 2 : 1);
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
	for (int size = 0; size < BallSizeCount; size++)
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
	for (int size = 0; size < BallSizeCount; size++)
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

	hInstance = m_hInstance;
	init_bitmap_file_count();
	init_hBitmap();
	set_BallSizeType(m_BallSizeType);

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
	for (int cur_object = 0; cur_object < object_count; cur_object++)
	{
		if (cur_object == object) break;
		idx += 
			count_table[cur_object][_texture] 
			* count_table[cur_object][_size]
			* (count_table[cur_object][_mask] ? 2 : 1);
	}

	//texture에 따른 idx 탐색
	for (UINT cur_texture = 0; cur_texture < count_table[object][_texture]; cur_texture++)
	{
		if (cur_texture == texture) break;
		idx +=
			count_table[object][_size] *
			(count_table[object][_mask] ? 2 : 1);
	}

	//mask 유무에 따른 idx 탐색
	if (mask == TRUE && count_table[object][_mask] == TRUE)
		idx += count_table[object][_size];

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
void BitmapManager::set_BallSizeType(int m_BallSizeType)
{
	BallSizeType = m_BallSizeType;
}
int BitmapManager::get_BallSizeType()
{
	return BallSizeType;
}
HBITMAP BitmapManager::get_hBitmap_floor()
{
	return oldvar_floor;
}
HBITMAP BitmapManager::get_hBitmap_ball()
{
	return oldvar_ball[BallSize_toIdx(BallSizeType)];
}
HBITMAP BitmapManager::get_hBitmap_ball_mask()
{
	return oldvar_ball_mask[BallSize_toIdx(BallSizeType)];
}
int BitmapManager::BallSize_toIdx(int BallSize)
{
	switch (BallSize)
	{
	case BallSize_small:
		return 0;
	case BallSize_medium:
		return 1;
	case BallSize_large:
		return 2;
	case BallSize_extra:
		return 3;
	default:
		return -1;
	}
}
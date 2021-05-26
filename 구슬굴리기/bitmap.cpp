#include "bitmap.h"
using namespace RollingBall;



/*****************************************
*
*	static variables initialization
* 
*****************************************/
int Bitmap::_file_count = 0;
HINSTANCE Bitmap::hInstance = NULL;
vector<HBITMAP> Bitmap::hBitmap = vector<HBITMAP>();



/*****************************************
*
*			private functions
* 
*****************************************/
void Bitmap::init_file_count()
{
	ObjectBitmapInfoVector v;
	if (!v.isLoaded()) return;

	_file_count = v.count_bitmap_files();
			
}
void Bitmap::init_hBitmap()
{
	if (!isInitFileCount()) return;
	if (isInitHBitmap())
		delete_hBitmap();

	for (int fileidx = 0; fileidx < _file_count; fileidx++)
		hBitmap.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(BMPFILEMACRO[fileidx])));
}
void Bitmap::delete_hBitmap()
{
	if (!isInitHBitmap()) return;
	for (int fileidx = 0; fileidx < _file_count; fileidx++)
		DeleteObject(hBitmap[fileidx]);
	
	hBitmap.clear();
}

BOOL Bitmap::isInitFileCount()
{
	return _file_count != 0;
}
BOOL Bitmap::isInitHBitmap()
{
	return hBitmap.size() != 0;
}


/*****************************************
*
*			public functions
*
*****************************************/
BOOL Bitmap::init(HINSTANCE m_hInstance)
{
	if (isInit()) return TRUE;

	ObjectBitmapInfoVector v;
	if (!v.isLoaded()) return FALSE;

	hInstance = m_hInstance;
	init_file_count();
	init_hBitmap();

	return TRUE;
}
BOOL Bitmap::isInit()
{
	return hInstance != NULL;
}
Bitmap::~Bitmap()
{
	if (isInit())
	{
		hInstance = NULL;
		delete_hBitmap();
	}
}


HBITMAP RollingBall::Bitmap::get(int index)
{
	if (!(0 <= index && index < file_count())) index = 0;
	return hBitmap[index];
}
HBITMAP RollingBall::Bitmap::get(Object& object, pixel texture_size, BOOL mask_texture)
{
	return hBitmap[idx(object, texture_size, mask_texture)];
}

HBITMAP RollingBall::Bitmap::operator()(int index)
{
	return get(index);
}

HBITMAP RollingBall::Bitmap::operator()(Object& object, pixel texture_size, BOOL mask_texture)
{
	return get(object, texture_size, mask_texture);
}

int RollingBall::Bitmap::idx(Object& object, pixel texture_size, BOOL mask_texture)
{
	int idx = 0;
	ObjectBitmapInfoVector v;
	int idx_object = object.index_object();
	//object에 따른 idx 탐색
	for (int curobjidx = 0; curobjidx < idx_object; curobjidx++) 
		idx += v.count_bitmap(curobjidx);

	//texture에 따른 idx 탐색
	idx += (v.count_bitmap(idx_object) / object.count_texture()) * object.index_texture();

	//mask 유무에 따른 idx 탐색
	if (mask_texture == TRUE) idx += object.count_texture_size();

	//size에 따른 idx 탐색
	idx += object.index_texture_size(texture_size);

	//구한 idx가 범위에 벗어나면 0으로 초기화
	if (!(0 <= idx && idx < file_count())) idx = 0;

	return idx;
}


HBITMAP Bitmap::create_hDC_compatible(HDC hdc, RECT& rt)
{
	return CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
}
void Bitmap::delete_hDC_compatible(HBITMAP hDCcompatibleBitmap)
{
	DeleteObject(hDCcompatibleBitmap);
}
int Bitmap::file_count()
{
	return _file_count;
}
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

/*
void Bitmap::arrange_idx(int& objidx, int& textureidx, int& sizeidx, BOOL& mask)
{
	if (!isIdxInRange(objidx, om.object_count())) objidx = 0;
	if (!isIdxInRange(textureidx, om.get_bmpInfo(objidx).count_texture())) textureidx = 0;
	if (!isIdxInRange(sizeidx, om.get_bmpInfo(objidx).count_texture_size())) sizeidx = 0;
	if (om.get_bmpInfo(objidx).has_mask() == FALSE && mask == TRUE) mask = FALSE;
} 
*/


/*****************************************
*
*			public functions
*
*****************************************/
BOOL Bitmap::init(HINSTANCE m_hInstance)
{
	ObjectBitmapInfoVector v;
	if (isInit()) return TRUE;
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
	if (0 <= index && index < file_count())
	return hBitmap[index];
}
HBITMAP RollingBall::Bitmap::get(Object& object, pixel texture_size)
{
	return HBITMAP();
}

HBITMAP RollingBall::Bitmap::operator()(Object& object, pixel texture_size)
{
	return HBITMAP();
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
	idx += object.index_texture(texture_size);

	//구한 idx가 범위에 벗어나면 0으로 초기화
	if (!(0 <= idx && idx < v.count_bitmap_files())) idx = 0;

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
/*
int Bitmap::get_curr_sel_idx()
{
	return index(
		curselidx.object, curselidx.texture, 
		curselidx.texture_size, curselidx.mask
	);
}
int Bitmap::get_curr_object_idx()
{
	return curselidx.object;
}
LPCTSTR Bitmap::get_curr_object_name()
{
	return om.get_bmpInfo(curselidx.object).name();
}

int Bitmap::get_curr_texture_idx()
{
	return curselidx.texture;
}
LPCTSTR Bitmap::get_curr_texture_name()
{
	return om.get_bmpInfo(curselidx.object).texture_name(curselidx.texture);
}

int Bitmap::get_curr_texture_size_idx()
{
	return curselidx.texture_size;
}
int Bitmap::get_curr_texture_size()
{
	return om.get_bmpInfo(curselidx.object).texture_size(curselidx.texture_size);
}

BOOL Bitmap::get_curr_object_has_mask()
{
	return om.get_bmpInfo(curselidx.object).has_mask();
}
*/
int Bitmap::file_count()
{
	return _file_count;
}



/*
int Bitmap::object(LPCTSTR m_obj)
{
	for (int i = 0; i < om.object_count(); i++) 
		if (_tcscmp(m_obj, om.get_bmpInfo(i).name()) == 0)
			return i;

	return 0;
}
LPCTSTR Bitmap::object(int m_objidx)
{
	return om.get_bmpInfo(m_objidx).name();
}

int Bitmap::texture(LPCTSTR m_texture)
{
	for (int i = 0; i < om.get_bmpInfo(curselidx.object).count_texture(); i++)
		if (_tcscmp(m_texture, om.get_bmpInfo(curselidx.object).texture_name(i)) == 0)
			return i;

	return 0;
}
LPCTSTR Bitmap::texture(int m_textureidx)
{
	return om.get_bmpInfo(curselidx.object).texture_name(m_textureidx);
}

int Bitmap::size(int m_size)
{
	for (int i = 0; i < om.get_bmpInfo(curselidx.object).count_texture_size(); i++)
		if (m_size == om.get_bmpInfo(curselidx.object).texture_size(i))
			return i;

	return 0;
}
int Bitmap::idx_to_size(int sizeidx)
{
	return om.get_bmpInfo(curselidx.object).texture_size(sizeidx);
}

void Bitmap::set_cur_sel(int objidx, int textureidx, int sizeidx, BOOL m_mask)
{
	curselidx.object = objidx;
	curselidx.texture = textureidx;
	curselidx.texture_size = sizeidx;
	curselidx.mask = m_mask;
	arrange_idx(curselidx.object, curselidx.texture, curselidx.texture_size, curselidx.mask);
}
void Bitmap::set_cur_sel(LPCTSTR m_obj, LPCTSTR m_texture, int m_size, BOOL m_mask)
{
	int objidx = object(m_obj);
	curselidx.object = objidx;
	int textureidx = texture(m_texture);
	curselidx.texture = textureidx;
	int sizeidx = size(m_size);
	set_cur_sel(objidx, textureidx, sizeidx, m_mask);
}
HBITMAP Bitmap::operator[](int index)
{
	return get(index);
}
*/
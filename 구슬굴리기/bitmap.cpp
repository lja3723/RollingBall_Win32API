#include "bitmap.h"
//#include "debugger.h"
using namespace RollingBall;



/*****************************************
*
*	static variables initialization
* 
*****************************************/
int Bitmap::bitmap_file_count = 0;
HINSTANCE Bitmap::hInstance = NULL;
vector<HBITMAP> Bitmap::hBitmap = vector<HBITMAP>();



/*****************************************
*
*			private functions
* 
*****************************************/
void Bitmap::init_bitmap_file_count()
{
	if (!om.isInitObjectInfo()) return;

	bitmap_file_count = 0;
	for (int obj = 0; obj < om.object_count(); obj++)
		bitmap_file_count += get_object_file_count(obj);
			
}
void Bitmap::init_hBitmap()
{
	if (!isInitBitmapFileCount()) return;
	if (isInitHBitmap())
		delete_hBitmap();

	for (int fileidx = 0; fileidx < bitmap_file_count; fileidx++)
		hBitmap.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(BMPFILEMACRO[fileidx])));
}
void Bitmap::delete_hBitmap()
{
	if (!isInitHBitmap()) return;
	for (int fileidx = 0; fileidx < bitmap_file_count; fileidx++)
		DeleteObject(hBitmap[fileidx]);
	
	hBitmap.clear();
}
void Bitmap::init_curselidx()
{
	set_cur_sel();
}

int Bitmap::get_object_file_count(int objidx)
{
	if (!(0 <= objidx && objidx < om.object_count())) objidx = 0;
	return (int)om.object_info(objidx).count_texture()
		* (int)om.object_info(objidx).count_texture_size()
		* (om.object_info(objidx).has_mask() ? 2 : 1);
}

BOOL Bitmap::isInitBitmapFileCount()
{
	return bitmap_file_count != 0;
}
BOOL Bitmap::isInitHBitmap()
{
	return hBitmap.size() != 0;
}

void Bitmap::arrange_idx(int& objidx, int& textureidx, int& sizeidx, BOOL& mask)
{
	if (!isIdxInRange(objidx, om.object_count())) objidx = 0;
	if (!isIdxInRange(textureidx, om.object_info(objidx).count_texture())) textureidx = 0;
	if (!isIdxInRange(sizeidx, om.object_info(objidx).count_texture_size())) sizeidx = 0;
	if (om.object_info(objidx).has_mask() == FALSE && mask == TRUE) mask = FALSE;
} 



/*****************************************
*
*			public functions
*
*****************************************/
BOOL Bitmap::init(HINSTANCE m_hInstance)
{
	if (isInit()) return TRUE;
	if (!om.isInitObjectInfo()) return FALSE;

#ifdef __Debugger_h__
	for (int i = 0; i < om.object_count(); i++)
	{
		tstring name, value;
		for (int j = 0; j < om.object_info(i).count_texture(); j++)
			name += om.object_info(i).texture_name(j), name += _T(" ");
		for (int j = 0; j < om.object_info(i).count_texture_size(); j++)
			value += std::to_wstring(om.object_info(i).texture_size(j)), value += _T(" ");

		debuggerMessage(
			"object index: %d\n"
			"object name: %s\n\n"

			"texture count: %d\n"
			"texture name: %s\n\n"

			"texture value count: %d\n"
			"values: %s\n\n"

			"has mask: %s",
			i, om.object_info(i).name(),
			(int)om.object_info(i).count_texture(), name.c_str(),
			(int)om.object_info(i).count_texture_size(), value.c_str(),
			(om.object_info(i).has_mask() ? _T("TRUE") : _T("FALSE"))
		);
	}
#endif

	hInstance = m_hInstance;
	init_bitmap_file_count();
	init_hBitmap();
	init_curselidx();

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

HBITMAP Bitmap::get(int index)
{
	if (isInitHBitmap()) return hBitmap[index];
	else return NULL;
}
HBITMAP Bitmap::create_hDC_compatible(HDC hdc, RECT& rt)
{
	return CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
}
void Bitmap::delete_hDC_compatible(HBITMAP hDCcompatibleBitmap)
{
	DeleteObject(hDCcompatibleBitmap);
}
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
	return om.object_info(curselidx.object).name();
}

int Bitmap::get_curr_texture_idx()
{
	return curselidx.texture;
}
LPCTSTR Bitmap::get_curr_texture_name()
{
	return om.object_info(curselidx.object).texture_name(curselidx.texture);
}

int Bitmap::get_curr_texture_size_idx()
{
	return curselidx.texture_size;
}
int Bitmap::get_curr_texture_size()
{
	return om.object_info(curselidx.object).texture_size(curselidx.texture_size);
}

BOOL Bitmap::get_curr_object_has_mask()
{
	return om.object_info(curselidx.object).has_mask();
}

int Bitmap::get_bitmap_file_count()
{
	return bitmap_file_count;
}




int Bitmap::index(int objidx, int textureidx, int sizeidx, BOOL m_mask)
{
	int idx = 0;

	arrange_idx(objidx, textureidx, sizeidx, m_mask);

	//object에 따른 idx 탐색
	for (int curobjidx = 0; curobjidx < objidx; curobjidx++) idx += get_object_file_count(curobjidx);

	//texture에 따른 idx 탐색
	idx += (get_object_file_count(objidx) / (int)om.object_info(objidx).count_texture()) * textureidx;

	//mask 유무에 따른 idx 탐색
	if (m_mask == TRUE) idx += (int)om.object_info(objidx).count_texture_size();

	//size에 따른 idx 탐색
	idx += sizeidx;

	//구한 idx가 범위에 벗어나면 0으로 초기화
	if (!(0 <= idx && idx < bitmap_file_count)) idx = 0;

	return idx;
}
int Bitmap::index(LPCTSTR m_obj, LPCTSTR m_texture, int m_size, BOOL m_mask)
{
	set_cur_sel(m_obj, m_texture, m_size, m_mask);
	return index(object(m_obj), texture(m_texture), size(m_size), m_mask);
}

int Bitmap::object(LPCTSTR m_obj)
{
	for (int i = 0; i < om.object_count(); i++) 
		if (_tcscmp(m_obj, om.object_info(i).name()) == 0)
			return i;

	return 0;
}
LPCTSTR Bitmap::object(int m_objidx)
{
	return om.object_info(m_objidx).name();
}

int Bitmap::texture(LPCTSTR m_texture)
{
	for (int i = 0; i < om.object_info(curselidx.object).count_texture(); i++)
		if (_tcscmp(m_texture, om.object_info(curselidx.object).texture_name(i)) == 0)
			return i;

	return 0;
}
LPCTSTR Bitmap::texture(int m_textureidx)
{
	return om.object_info(curselidx.object).texture_name(m_textureidx);
}

int Bitmap::size(int m_size)
{
	for (int i = 0; i < om.object_info(curselidx.object).count_texture_size(); i++)
		if (m_size == om.object_info(curselidx.object).texture_size(i))
			return i;

	return 0;
}
int Bitmap::idx_to_size(int sizeidx)
{
	return om.object_info(curselidx.object).texture_size(sizeidx);
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


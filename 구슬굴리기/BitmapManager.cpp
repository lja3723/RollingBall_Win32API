#include "BitmapManager.h"
//#include "Debugger.h"
using namespace RollingBall;



/*****************************************
*
*	static variables initialization
* 
*****************************************/
int BitmapManager::bitmap_file_count = 0;
HINSTANCE BitmapManager::hInstance = NULL;
vector<BitmapManager::BitmapManagerObject> BitmapManager::object_info 
	= vector<BitmapManager::BitmapManagerObject>();
vector<HBITMAP> BitmapManager::hBitmap = vector<HBITMAP>();


//old variables below
//HBITMAP BitmapManager::oldvar_floor = NULL;
//HBITMAP BitmapManager::oldvar_ball[old_BallSizeCount] = { NULL, };
//HBITMAP BitmapManager::oldvar_ball_mask[old_BallSizeCount] = { NULL, };



/*****************************************
*
*			private functions
* 
*****************************************/
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
BOOL RollingBall::BitmapManager::init_object_info(HWND m_hwnd)
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
		MessageBox(m_hwnd, errmsg, _T("오류"), MB_OK);
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
void RollingBall::BitmapManager::init_bitmap_file_count()
{
	if (!isInitObjectInfo()) return;

	bitmap_file_count = 0;
	for (int obj = 0; obj < object_info.size(); obj++)
		bitmap_file_count += get_file_count(obj);
			
}
void RollingBall::BitmapManager::init_hBitmap()
{
	if (!isInitBitmapFileCount()) return;
	if (isInitHBitmap())
		delete_hBitmap();

	for (int fileidx = 0; fileidx < bitmap_file_count; fileidx++)
		hBitmap.push_back(LoadBitmap(hInstance, MAKEINTRESOURCE(BMPFILEMACRO[fileidx])));

	//old code below
	//비트맵 로드
	/*
	oldvar_floor = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_FLOOR_WOOD1_256));
	for (int size = 0; size < old_BallSizeCount; size++)
	{
		oldvar_ball[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BALL_IRON1_032 + 2 * size));
		oldvar_ball_mask[size] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BALL_IRON1_032M + 2 * size));
	}
	*/
}
void RollingBall::BitmapManager::delete_hBitmap()
{
	if (!isInitHBitmap()) return;
	for (int fileidx = 0; fileidx < bitmap_file_count; fileidx++)
		DeleteObject(hBitmap[fileidx]);
	
	hBitmap.clear();

	//old code below
	/*
	DeleteObject(oldvar_floor);
	for (int size = 0; size < old_BallSizeCount; size++)
	{
		DeleteObject(oldvar_ball[size]);
		DeleteObject(oldvar_ball_mask[size]);
	}
	*/
}
void RollingBall::BitmapManager::init_curselidx()
{
	set_cur_sel();
}
int RollingBall::BitmapManager::get_file_count(int objidx)
{
	if (!(0 <= objidx && objidx < object_info.size())) objidx = 0;
	return (int)object_info[objidx].texture.name.size()
		* (int)object_info[objidx].texture.value.size()
		* (object_info[objidx].has_mask ? 2 : 1);
}

BOOL RollingBall::BitmapManager::isInitObjectInfo()
{
	return object_info.size() != 0;
}
BOOL RollingBall::BitmapManager::isInitBitmapFileCount()
{
	return bitmap_file_count != 0;
}
BOOL RollingBall::BitmapManager::isInitHBitmap()
{
	return hBitmap.size() != 0;
}

void RollingBall::BitmapManager::arrange_idx(int& objidx, int& textureidx, int& sizeidx, BOOL& mask)
{
	if (!(0 <= objidx && objidx < object_info.size())) objidx = 0;
	if (!(0 <= textureidx && textureidx < object_info[objidx].texture.name.size())) textureidx = 0;
	if (!(0 <= sizeidx && sizeidx < object_info[objidx].texture.value.size())) sizeidx = 0;
	if (object_info[objidx].has_mask == FALSE && mask == TRUE) mask = FALSE;
} 



/*****************************************
*
*			public functions
*
*****************************************/
BOOL BitmapManager::init(HINSTANCE m_hInstance, HWND m_hwnd)
{
	if (isInit()) return TRUE;
	if (!init_object_info(m_hwnd)) return FALSE;

#ifdef __Debugger_h__
	for (int i = 0; i < object_info.size(); i++)
	{
		tstring name, value;
		for (int j = 0; j < object_info[i].texture.name.size(); j++)
			name += object_info[i].texture.name[j], name += _T(" ");
		for (int j = 0; j < object_info[i].texture.value.size(); j++)
			value += std::to_wstring(object_info[i].texture.value[j]), value += _T(" ");

		debuggerMessage(
			"object index: %d\n"
			"object name: %s\n\n"

			"texture count: %d\n"
			"texture name: %s\n\n"

			"texture value count: %d\n"
			"values: %s\n\n"

			"has mask: %s",
			i, object_info[i].name.c_str(),
			object_info[i].texture.name.size(), name.c_str(),
			object_info[i].texture.value.size(), value.c_str(),
			(object_info[i].has_mask ? _T("TRUE") : _T("FALSE"))
		);
	}
#endif

	hInstance = m_hInstance;
	hwnd = m_hwnd;
	init_bitmap_file_count();
	init_hBitmap();
	init_curselidx();
	//old_set_BallSizeType(m_BallSizeType);

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
	if (isInitHBitmap()) return hBitmap[index];
	else return NULL;
}
HBITMAP RollingBall::BitmapManager::create_hDC_compatible(HDC hdc, RECT& rt)
{
	return CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
}
void RollingBall::BitmapManager::delete_hDC_compatible(HBITMAP hDCcompatibleBitmap)
{
	DeleteObject(hDCcompatibleBitmap);
}
int RollingBall::BitmapManager::get_curr_object_idx()
{
	return curselidx.object;
}
LPCTSTR RollingBall::BitmapManager::get_curr_object_name()
{
	return object_info[curselidx.object].name.c_str();
}
int RollingBall::BitmapManager::get_curr_texture_idx()
{
	return curselidx.texture;
}
LPCTSTR RollingBall::BitmapManager::get_curr_texture_name()
{
	return object_info[curselidx.object].texture.name[curselidx.texture].c_str();
}
int RollingBall::BitmapManager::get_curr_texture_size_idx()
{
	return curselidx.texture_size;
}
int RollingBall::BitmapManager::get_curr_texture_size()
{
	return object_info[curselidx.object].texture.value[curselidx.texture_size];
}
BOOL RollingBall::BitmapManager::get_curr_object_has_mask()
{
	return object_info[curselidx.object].has_mask;
}




int RollingBall::BitmapManager::index(int objidx, int textureidx, int sizeidx, BOOL m_mask)
{
	int idx = 0;

	arrange_idx(objidx, textureidx, sizeidx, m_mask);

	//object에 따른 idx 탐색
	for (int curobjidx = 0; curobjidx < objidx; curobjidx++) idx += get_file_count(curobjidx);

	//texture에 따른 idx 탐색
	idx += (get_file_count(objidx) / (int)object_info[objidx].texture.name.size()) * textureidx;

	//mask 유무에 따른 idx 탐색
	if (m_mask == TRUE) idx += (int)object_info[objidx].texture.value.size();

	//size에 따른 idx 탐색
	idx += sizeidx;

	//구한 idx가 범위에 벗어나면 0으로 초기화
	if (!(0 <= idx && idx < bitmap_file_count)) idx = 0;

	return idx;
}
int RollingBall::BitmapManager::index(LPCTSTR m_obj, LPCTSTR m_texture, int m_size, BOOL m_mask)
{
	set_cur_sel(m_obj, m_texture, m_size, m_mask);
	return index(object(m_obj), texture(m_texture), size(m_size), m_mask);
}

int RollingBall::BitmapManager::object(LPCTSTR m_obj)
{
	for (int i = 0; i < object_info.size(); i++) 
		if (_tcscmp(m_obj, object_info[i].name.c_str()) == 0)
			return i;

	return 0;
}
LPCTSTR RollingBall::BitmapManager::object(int m_objidx)
{
	return object_info[m_objidx].name.c_str();
}

int RollingBall::BitmapManager::texture(LPCTSTR m_texture)
{
	for (int i = 0; i < object_info[curselidx.object].texture.name.size(); i++)
		if (_tcscmp(m_texture, object_info[curselidx.object].texture.name[i].c_str()) == 0)
			return i;

	return 0;
}
LPCTSTR RollingBall::BitmapManager::texture(int m_textureidx)
{
	return object_info[curselidx.object].texture.name[m_textureidx].c_str();
}

int RollingBall::BitmapManager::size(int m_size)
{
	for (int i = 0; i < object_info[curselidx.object].texture.value.size(); i++)
		if (m_size == object_info[curselidx.object].texture.value[i])
			return i;

	return 0;
}
int RollingBall::BitmapManager::idx_to_size(int sizeidx)
{
	return object_info[curselidx.object].texture.value[sizeidx];
}

void RollingBall::BitmapManager::set_cur_sel(int objidx, int textureidx, int sizeidx, BOOL m_mask)
{
	curselidx.object = objidx;
	curselidx.texture = textureidx;
	curselidx.texture_size = sizeidx;
	curselidx.mask = m_mask;
	arrange_idx(curselidx.object, curselidx.texture, curselidx.texture_size, curselidx.mask);
}
void RollingBall::BitmapManager::set_cur_sel(LPCTSTR m_obj, LPCTSTR m_texture, int m_size, BOOL m_mask)
{
	int objidx = object(m_obj);
	int textureidx = texture(m_texture);
	int sizeidx = size(m_size);
	set_cur_sel(objidx, textureidx, sizeidx, m_mask);
}
HBITMAP RollingBall::BitmapManager::operator[](int index)
{
	return get(index);
}




//old functions below
/*
void BitmapManager::old_set_BallSizeType(int m_BallSizeType)
{
	old_BallSizeType = m_BallSizeType;
}
int BitmapManager::old_get_BallSizeType()
{
	return old_BallSizeType;
}
HBITMAP BitmapManager::old_get_hBitmap_floor()
{
	return oldvar_floor;
}
HBITMAP BitmapManager::old_get_hBitmap_ball()
{
	return get(index(_T("old_ball"), _T("iron1"), 64, FALSE));
}
HBITMAP BitmapManager::old_get_hBitmap_ball_mask()
{
	return get(index(_T("old_ball"), _T("iron1"), 64, TRUE));
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
*/
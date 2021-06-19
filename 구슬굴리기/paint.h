#pragma once
#ifndef __paint_h__
#define __paint_h__

#include <Windows.h>
#include <vector>
#include "bitmap.h"
#include "object.h"
#include "scaler.h"
#include "event.h"

#include "paint_hdc.h"
#include "paint_hbitmap.h"

using std::vector;

/*
*
* Paint:
* 프로그램의 시각적 표현을 전담하는 클래스를 정의
*
*/

namespace RollingBall
{
	class Paint : public EventAcceptable
	{
	private:
		Scaler scale;
		HINSTANCE hInstance;
		HWND hwnd;
		RECT windowRect;

		Paint_hDC hDC;
		Paint_hBitmap hBitmap;




		//여러가지 플래그를 저장하는 변수
		class _flag{
		public:
			BOOL isDoubleBufferingStart;
			BOOL isInitDoubleBuffering;
			BOOL isWindowSizeChanged;

		public:
			_flag() {
				isDoubleBufferingStart = FALSE;
				isInitDoubleBuffering = FALSE;
				isWindowSizeChanged = FALSE;
			}
		} flag;
	
	public:
		Paint() {
			hInstance = NULL;
			hwnd = NULL;
			memset(&windowRect, 0, sizeof(windowRect));

			//oldcode
			//_oldcode_flag_isSetHBitmapRes = FALSE;
			//_oldcode_flag_isBackedUpHBitmapRes = FALSE;
			//~oldcode
		}
		~Paint();

		//PrantManager 클래스 변수를 사용하기 전 반드시 수행해야 한다
		BOOL init(HINSTANCE m_hInstance, HWND m_hwnd);
	
		//scaler 설정 함수집합
		void set_px_rate(pixel px_rate);
		void set_fix_point(PhysicalVector fix_point);
		void set_fix_point(PixelCoord fix_point);

		//페인트를 시작한다
		void begin();

		//페인트를 마친다
		void end();

		//배경을 페인트한다
		void background(Object& background);

		//오브젝트를 페인트한다
		void operator()(Object& obj);

		//오브젝트 정보를 페인트한다
		void info(Object& obj, int yPos = 0);

		void text(LPCTSTR text, pixel x, pixel y);
	
		//////////////////////////////////
		//oldcode
		//////////////////////////////////
		/*
	private:
		Bitmap _oldcode_bmp;
		//리소스 개수를 저장함
		//static int _oldcode_res_count;

		struct __oldcode_hBitmap {
			HBITMAP windowBuffer;
			vector<HBITMAP> res;
			struct _old {
				HBITMAP windowBuffer;
				vector<HBITMAP> res;
			} old;
		} _oldcode_hBitmap;

		BOOL _oldcode_flag_isSetHBitmapRes;
		BOOL _oldcode_flag_isBackedUpHBitmapRes;

		//void _oldcode_init_res_count();
		//변수가 세팅되었는지 알려줌
		BOOL _oldcode_isSetHBitmapWindowBuffer();
		BOOL _oldcode_isSetHBitmapRes();
		BOOL _oldcode_isBackedUpHBitmapWindowBuffer();
		BOOL _oldcode_isBackedUpHBitmapRes();

		//hBitmap 변수를 관리함
		void _oldcode_hBitmap_windowBuffer_init();
		void _oldcode_hBitmap_windowBuffer_set________2();
		void _oldcode_hBitmap_windowBuffer_release();
		void _oldcode_hBitmap_res_init();
		void _oldcode_hBitmap_res_set();

		//hBitmap.old 변수를 관리함
		void _oldcode_hBitmap_old_windowBuffer_init();
		void _oldcode_hBitmap_old_windowBuffer_backup________3();
		void _oldcode_hBitmap_old_windowBuffer_rollback();
		void _oldcode_hBitmap_old_res_init();
		void _oldcode_hBitmap_old_res_backup______1();
		void _oldcode_hBitmap_old_res_rollback();
		*/
		//////////////////////////////////
		//~oldcode
		//////////////////////////////////
	

	private:
		/********************************
		*
		*	init management
		*
		*********************************/
		//클래스의 각종 플래그변수를 초기화
		//Paint::init()에서만 호출되어야 함
		void init_res_vectors(int res_count);



		/********************************
		*
		*	bool returns
		*
		*********************************/
		//기타 정보를 알려줌
		BOOL isInit();
		BOOL isDoubleBufferingStart();
		BOOL isReadyToPaint();
		BOOL isInitDoubleBuffering();
		BOOL isWindowSizeChanged();



		/********************************
		*
		*	double buffering management
		*
		*********************************/
		//더블버퍼링을 처음으로 시작하기 위한 준비를 함
		void doubleBuffering_init();
		//더블버퍼링을 시작하고 종료함
		void doubleBuffering_start();
		void doubleBuffering_stop();
		//프로그램을 종료할 때 진행함
		void doubleBuffering_halt();



		/********************************
		*
		*	paint management
		*
		*********************************/
		//오브젝트를 버퍼에 그림
		void paint_background_tobuffer(Object& background);
		void paint_background_ruller_tobuffer();
		//void paint_ball_tobuffer(int x, int y, int ballsize = 64);
		void paint_tobuffer(Object& object);

		void paint_info_tobuffer(Object& object, int yPos = 0);

		void paint_text_tobuffer(LPCTSTR text, pixel x, pixel y);
		//버퍼에 그려진 그림을 윈도우로 출력
		void flush_buffer();




		/********************************
		*
		*	event 처리
		*
		*********************************/
		//기타 이벤트 처리
		virtual void event_all(Event e);
	};
}

#endif
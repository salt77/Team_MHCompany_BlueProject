#pragma once
#ifndef __KeyManager_H__
#define __KeyManager_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyManager : public CBase
{
	DECLARE_SINGLETON(CKeyManager)

private:
	CKeyManager();
	virtual ~CKeyManager();

public:
	virtual void	Free();

public:
	void UpdateKeyManager(HWND hWnd);

	bool Key_Up(long long llKey);

	bool Key_Down(long long llKey);

	bool Key_Pressing(long long llKey);

	_vec2 Get_MouseMovementFigure() { return _vec2(m_fThisFrameMouseMoveX, m_fThisFrameMouseMoveY); }
private:
	void	Key_Setting();
	void	Check_Key(_uint Key, long long CustomKey);
	void	Check_MouseMove(HWND hWnd);

private:
	long long m_llKey;
	long long m_llKeyDown;
	long long m_llKeyDownUpdate;
	long long m_llKeyUp;
	long long m_llKeyUpUpdate;


	// 이번 프레임 마우스 움직임 저장
private:
	POINT		m_ptMousePre;
	POINT		m_ptMouseCur;
	_float		m_fThisFrameMouseMoveX = 0.f;
	_float		m_fThisFrameMouseMoveY = 0.f;
};


END
#endif // __KeyManager_H__

#define KEY_F1			((long long)1) << 0 
#define KEY_F2			((long long)1) << 1 
#define KEY_F3			((long long)1) << 2 
#define KEY_F4			((long long)1) << 3 
#define KEY_F5			((long long)1) << 4 
#define KEY_F6			((long long)1) << 5 
#define KEY_F7			((long long)1) << 6 
#define KEY_F8			((long long)1) << 7 
#define KEY_F9			((long long)1) << 8 
#define KEY_F10			((long long)1) << 9 
#define KEY_F11			((long long)1) << 10
#define KEY_F12			((long long)1) << 11
#define KEY_LBUTTON		((long long)1) << 12
#define KEY_RBUTTON		((long long)1) << 13
#define KEY_UP			((long long)1) << 14
#define KEY_DOWN		((long long)1) << 15
#define KEY_LEFT		((long long)1) << 16
#define KEY_RIGHT		((long long)1) << 17
#define KEY_SPACE		((long long)1) << 18
#define KEY_ESCAPE		((long long)1) << 19
#define KEY_1			((long long)1) << 20
#define KEY_2			((long long)1) << 21
#define KEY_3			((long long)1) << 22
#define KEY_4			((long long)1) << 23
#define KEY_5			((long long)1) << 24
#define KEY_6			((long long)1) << 25
#define KEY_7			((long long)1) << 26
#define KEY_8			((long long)1) << 27
#define KEY_9			((long long)1) << 28
#define KEY_0			((long long)1) << 29
#define KEY_Q			((long long)1) << 30
#define KEY_W			((long long)1) << 31
#define KEY_E			((long long)1) << 32
#define KEY_Y			((long long)1) << 33
#define KEY_R			((long long)1) << 34
#define KEY_T			((long long)1) << 35
#define KEY_U			((long long)1) << 36
#define KEY_I			((long long)1) << 37
#define KEY_O			((long long)1) << 38
#define KEY_P			((long long)1) << 39
#define KEY_A			((long long)1) << 40
#define KEY_S			((long long)1) << 41
#define KEY_D			((long long)1) << 42
#define KEY_F			((long long)1) << 43
#define KEY_G			((long long)1) << 44
#define KEY_H			((long long)1) << 45
#define KEY_J			((long long)1) << 46
#define KEY_K			((long long)1) << 47
#define KEY_L			((long long)1) << 48
#define KEY_Z			((long long)1) << 49
#define KEY_X			((long long)1) << 50
#define KEY_C			((long long)1) << 51
#define KEY_V			((long long)1) << 52
#define KEY_B			((long long)1) << 53
#define KEY_N			((long long)1) << 54
#define KEY_M			((long long)1) << 55
#define KEY_Shift		((long long)1) << 56
#define KEY_Alt			((long long)1) << 57
#define KEY_Delete		((long long)1) << 58
#define KEY_Tab			((long long)1) << 59
#define KEY_PLUS		((long long)1) << 60
#define KEY_MINUS		((long long)1) << 61
#define KEY_Back		((long long)1) << 62
#define KEY_Enter		((long long)1) << 63
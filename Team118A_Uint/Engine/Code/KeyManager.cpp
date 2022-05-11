//#include "stdafx.h"
#include "KeyManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CKeyManager)

Engine::CKeyManager::CKeyManager()
	: m_llKey(0)
	, m_llKeyDown(0)
	, m_llKeyDownUpdate(0)
	, m_llKeyUp(0)
	, m_llKeyUpUpdate(0)
{
}

Engine::CKeyManager::~CKeyManager()
{
	Free();
}

void Engine::CKeyManager::Free()
{
}

// 키 셋팅값의 초기화, 키 다운,업의 값 저장 
void CKeyManager::UpdateKeyManager(HWND hWnd)
{
	// 키 초기화
	m_llKeyDown |= m_llKeyDownUpdate;
	m_llKeyDownUpdate = 0;

	m_llKeyUp &= m_llKeyUpUpdate;
	m_llKeyUpUpdate = 0;

	m_llKey = 0;

	// 키 셋팅
	Key_Setting();

	// 마우스 움직임 체크
	Check_MouseMove(hWnd);
}

// bool로 이번 프레임에 키를 땟는지(true) 판단, 아니면 (false)
bool CKeyManager::Key_Up(long long llKey)
{
	if (m_llKey & llKey)
	{
		m_llKeyUp |= llKey;
		m_llKeyUpUpdate |= llKey;
		return false;
	}
	else if (m_llKeyUp & llKey)
	{
		m_llKeyUp = 0;
		m_llKeyUpUpdate ^= llKey;
		return true;
	}

	return false;
}

// bool로 이번 프레임에 키를 눌렀는지(true) 판단, 아니면 (false)
bool CKeyManager::Key_Down(long long llKey)
{
	if ((m_llKey & llKey) && !(m_llKeyDown & llKey))
	{
		m_llKeyDownUpdate |= llKey;
		return true;
	}
	else if (!(m_llKey & llKey) && (m_llKeyDown & llKey))
	{
		m_llKeyDown ^= llKey;
		return false;
	}
	return false;
}

// 해당 키가 눌리고 있는 지(true) 판단, 아니면 (false)
bool CKeyManager::Key_Pressing(long long llKey)
{
	if (m_llKey & llKey)
		return true;

	return false;
}

// 키값에 대한 정의 (VK_키) 값을 (KEY_키) 값으로 만들어준다
void CKeyManager::Key_Setting()
{
	Check_Key(VK_LBUTTON, KEY_LBUTTON);
	Check_Key(VK_RBUTTON, KEY_RBUTTON);
	Check_Key(VK_UP, KEY_UP);
	Check_Key(VK_DOWN, KEY_DOWN);
	Check_Key(VK_LEFT, KEY_LEFT);
	Check_Key(VK_RIGHT, KEY_RIGHT);
	Check_Key(VK_SPACE, KEY_SPACE);
	Check_Key(VK_ESCAPE, KEY_ESCAPE);
	Check_Key('1', KEY_1);
	Check_Key('2', KEY_2);
	Check_Key('3', KEY_3);
	Check_Key('4', KEY_4);
	Check_Key('5', KEY_5);
	Check_Key('6', KEY_6);
	Check_Key('7', KEY_7);
	Check_Key('8', KEY_8);
	Check_Key('9', KEY_9);
	Check_Key('0', KEY_0);
	Check_Key(VK_F1, KEY_F1);
	Check_Key(VK_F2, KEY_F2);
	Check_Key(VK_F3, KEY_F3);
	Check_Key(VK_F4, KEY_F4);
	Check_Key(VK_F5, KEY_F5);
	Check_Key(VK_F6, KEY_F6);
	Check_Key(VK_F7, KEY_F7);
	Check_Key(VK_F8, KEY_F8);
	Check_Key(VK_F9, KEY_F9);
	Check_Key(VK_F10, KEY_F10);
	Check_Key(VK_F11, KEY_F11);
	Check_Key(VK_F12, KEY_F12);
	Check_Key('Q', KEY_Q);
	Check_Key('W', KEY_W);
	Check_Key('E', KEY_E);
	Check_Key('R', KEY_R);
	Check_Key('T', KEY_T);
	Check_Key('Y', KEY_Y);
	Check_Key('U', KEY_U);
	Check_Key('I', KEY_I);
	Check_Key('O', KEY_O);
	Check_Key('P', KEY_P);
	Check_Key('A', KEY_A);
	Check_Key('S', KEY_S);
	Check_Key('D', KEY_D);
	Check_Key('F', KEY_F);
	Check_Key('G', KEY_G);
	Check_Key('H', KEY_H);
	Check_Key('J', KEY_J);
	Check_Key('K', KEY_K);
	Check_Key('L', KEY_L);
	Check_Key('Z', KEY_Z);
	Check_Key('X', KEY_X);
	Check_Key('C', KEY_C);
	Check_Key('V', KEY_V);
	Check_Key('B', KEY_B);
	Check_Key('N', KEY_N);
	Check_Key('M', KEY_M);
	Check_Key(VK_SHIFT, KEY_Shift);
	Check_Key(VK_MENU, KEY_Alt);
	Check_Key(VK_DELETE, KEY_Delete);
	Check_Key(VK_TAB, KEY_Tab);
	Check_Key(VK_ADD, KEY_PLUS);
	Check_Key(VK_SUBTRACT, KEY_MINUS); 
	Check_Key(VK_BACK, KEY_Back);
	Check_Key(VK_RETURN, KEY_Enter);
}

void CKeyManager::Check_Key(_uint Key, long long CustomKey)
{
	if (GetAsyncKeyState(Key) & 0x8000)
		m_llKey |= CustomKey;
}

void CKeyManager::Check_MouseMove(HWND hWnd)
{
	POINT ptMouse = CUtility::Get_MousePoint(hWnd);

	m_ptMouseCur = ptMouse;

	m_fThisFrameMouseMoveX = (_float)(m_ptMouseCur.x - m_ptMousePre.x);
	m_fThisFrameMouseMoveY = (_float)(m_ptMouseCur.y - m_ptMousePre.y);

	m_ptMousePre = m_ptMouseCur;
}

// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include <iostream>
#define MAX_LOADSTRING 100

// 전역 변수:
HWND g_hWnd;
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	AllocConsole();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;
	msg.message = WM_NULL;

	CMainApp*		pMainApp = CMainApp::Create();
	assert(pMainApp);

	FAILED_CHECK_RETURN(Management Ready_Timer(L"Timer"), E_FAIL);
	FAILED_CHECK_RETURN(Management Ready_Frame(L"Frame60", 60.f), E_FAIL);
	FAILED_CHECK_RETURN(Management Ready_Frame(L"Frame120", 120.f), E_FAIL);
	FAILED_CHECK_RETURN(Management Ready_Frame(L"FrameNoLimit", 9999.f), E_FAIL);


	// 기본 메시지 루프입니다.
	_int iFPS = 0;
	_float Timer_Immediate = 0.f;
	_float fTimeDelta = 0.f;
	_float TimeAcc = 0.f;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Management Set_TimeDelta(L"Timer");
			Timer_Immediate = Management Get_TimeDelta(L"Timer");
			fTimeDelta = Management Get_AccTimeDelta(L"Frame60");
			TimeAcc += Timer_Immediate;
			if (Management IsPermit_Call(L"Frame60", Timer_Immediate))
			{
				iFPS++;

				pMainApp->Update_Main(fTimeDelta);
				//pMainApp->Update_Main(Timer_Immediate);

				if (TimeAcc >= 1.f)
				{
					_tchar szText[MAX_PATH]{ };
					swprintf_s(szText, L"(60프레임 제한)   FPS : %i", iFPS);
					SetWindowText(g_hWnd, szText);
					iFPS = 0;
					TimeAcc = 0.f;
				}
			}
			else
			{

				_int iTime = 0;
				iTime = 1;
			}
		}
	}

	_ulong	dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(pMainApp))
	{
		assert(FALSE);
	}
	FreeConsole();

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	RECT tRect = { 0,0,WINCX,WINCY };

	AdjustWindowRect(&tRect, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hWnd = CreateWindowW(szWindowClass, 
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		0, 
		tRect.right - tRect.left, 
		tRect.bottom - tRect.top, 
		nullptr, 
		nullptr, 
		hInstance, 
		nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	g_hWnd = hWnd;
	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// ESC를 KEY_DOWN하면 프로그램 종료
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


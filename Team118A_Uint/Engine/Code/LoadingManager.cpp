#include "LoadingManager.h"
#include "UI_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLoadingManager)


CLoadingManager::CLoadingManager()
{

}

CLoadingManager::~CLoadingManager()
{
	Free();
}

void CLoadingManager::Free()
{
	Safe_Release(m_pUIObject);
}

void CLoadingManager::Set_Loading(_bool bLoading)
{
	if (!m_bLoading && bLoading)
	{
		m_bLoading = bLoading;

		if (!m_pUIObject)
		{
			m_pUIObject = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Test_Loading.uidat");
		}
		m_HandleThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&m_dwThreadID);
	}
	else if (m_bLoading && !bLoading)
	{
		m_bLoading = bLoading;
		if (m_pUIObject)
		{
			Safe_Release(m_pUIObject);
			m_pUIObject = nullptr;
		}
	}

}

void CLoadingManager::Loading_Render()
{
	if (m_pUIObject)
	{
		m_pUIObject->Update_Object(0.01f);
		
		m_pUIObject->Render_Object(0);

	}
}

unsigned __stdcall CLoadingManager::ThreadFunction(void * pArg)
{
	while (TRUE)
	{
		if (!CLoadingManager::GetInstance()->Get_Loading())
		{
			break;
		}

		CGraphicDev::GetInstance()->GetDevice()->Clear(0,
			nullptr,
			D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DXCOLOR(0.f, 0.f, 0.f, 1.f),
			1.f,
			0);

		CGraphicDev::GetInstance()->GetDevice()->BeginScene();

		//±×¸®±â
		CLoadingManager::GetInstance()->Loading_Render();
		

		CGraphicDev::GetInstance()->GetDevice()->EndScene();
		CGraphicDev::GetInstance()->GetDevice()->Present(NULL, NULL, *CUI_Manager::GetInstance()->Get_Hwnd(), NULL);
	}

	return 0;
}



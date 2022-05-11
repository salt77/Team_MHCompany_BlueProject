#include "WorldUI.h"
#include "UI_Manager.h"
USING(Engine)


CWorldUI::CWorldUI(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUIPrefabPath)
	:CGameObject(pGraphicDev, eObjTag),
	m_wstrUIPrefabPath(wstrUIPrefabPath)
{
}

CWorldUI::CWorldUI(const CWorldUI & rhs)
	:CGameObject(rhs)
{
}

CWorldUI::~CWorldUI()
{
}

CWorldUI * CWorldUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUIPrefabPath)
{
	CWorldUI* pInstance = new CWorldUI(pGraphicDev, eObjTag, wstrUIPrefabPath);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CWorldUI::Free()
{
	Safe_Release(m_pUIObj);
	CGameObject::Free();
}

HRESULT CWorldUI::Ready_Object()
{
	CGameObject::Ready_Object();

	m_wstrUIPrefabPath = L"../../Reference/Data/Data_UI/Prefab/" + m_wstrUIPrefabPath;
	m_pUIObj = CUI_Manager::GetInstance()->Load_UI_Prefab(m_wstrUIPrefabPath);
	if (!m_pUIObj)
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CWorldUI::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	_vec2 vUIPos = CUtility::World2Ui(Get_Position());
	m_pUIObj->Set_Ui_PosX(_int(vUIPos.x+m_vUILocalPos.x));
	m_pUIObj->Set_Ui_PosY(_int(vUIPos.y+ m_vUILocalPos.y));

	m_pUIObj->Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CWorldUI::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);
	m_pUIObj->LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CWorldUI::Render_Object(_int iPass)
{
	
}

void CWorldUI::Add_Component()
{

}

void CWorldUI::Set_Constantable()
{
}

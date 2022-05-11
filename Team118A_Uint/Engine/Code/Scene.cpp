#include "Scene.h"

#include "GameObject.h"
#include "ActionCam_Mgr.h"
#include "SkyDome.h"
#include "UI_Manager.h"
#include "Skill_Manager.h"
#include "LoadingManager.h"

CScene*		CScene::m_pMainScene = nullptr;
_float		CScene::m_fPlayTime = 0.f;

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

void CScene::Free()
{
	_int iSize = (_int)m_vecNextFrame.size();
	for (_int i = 0; i < iSize; ++i)
	{
		Add_GameObject(m_vecNextFrame[i].first, m_vecNextFrame[i].second.first, m_vecNextFrame[i].second.second);
	}
	m_vecNextFrame.clear();


	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	CActionCam_Mgr::GetInstance()->Scene_Change();
	Safe_Release(m_pGraphicDev);

	m_pSkySphere = nullptr;
}

void CScene::Add_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, CGameObject * pGameObject)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
	{
		return;
	}

	iter->second->Add_GameObject(eObjTag, pGameObject);
	pGameObject->Set_LayerTag(eLayerTag); 
}

void CScene::Add_GameObjectNextFrame_ForLoading(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, CGameObject * pGameObject)
{
	m_vecNextFrame.push_back(make_pair(eLayerTag, make_pair(eObjTag, pGameObject)));
}

void CScene::Add_Layer(E_LAYERTAG eLayerTag, CLayer * pLayer)
{
	assert(FALSE == (eLayerTag == E_LAYERTAG::TAG_END));

	pLayer->Set_LayerTag(eLayerTag);

	m_mapLayer.emplace(eLayerTag, pLayer);
}

CLayer * CScene::Get_Layer(E_LAYERTAG eLayerTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
	{
		return nullptr;
	}
	return iter->second;
}

CComponent * CScene::Get_Component(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx, const wstring & pComponentTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
	{
		return nullptr;
	}

	return iter->second->Get_Component(eObjTag, iIdx, pComponentTag);
}

CGameObject * CScene::Get_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(eObjTag, iIdx);
}

vector<CGameObject*>* CScene::Get_vecGameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_vecGameObject(eObjTag);
}

map<E_OBJTAG, vector<CGameObject*>>* CScene::Get_LayerMapObject(E_LAYERTAG eLayerTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_LayerMapObject();
} 

void CScene::Set_Dead_AllObject(E_LAYERTAG eLayerTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return;

	map<E_OBJTAG, vector<CGameObject*>>* pLayerMap = iter->second->Get_LayerMapObject();

	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObj : tPair.second)
		{
			pObj->Set_Dead(TRUE);
		}
	}
}

void CScene::Set_Dead_CafeTile(E_LAYERTAG eLayerTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return;

	map<E_OBJTAG, vector<CGameObject*>>* pLayerMap = iter->second->Get_LayerMapObject();

	for (auto& tPair : *pLayerMap)
	{
		if (tPair.first == E_OBJTAG::OBJ_CafeTile)
		{
			for (auto& pObj : tPair.second)
			{
				pObj->Set_Dead(TRUE);
			}
		}
	}
}

void CScene::Set_Dead_CafeWall_L(E_LAYERTAG eLayerTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return;

	map<E_OBJTAG, vector<CGameObject*>>* pLayerMap = iter->second->Get_LayerMapObject();

	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObj : tPair.second)
		{
			if (pObj->Get_Name() == L"내츄럴 나무 벽_Left.X" ||
				pObj->Get_Name() == L"패턴 타일 벽_Left.X" ||
				pObj->Get_Name() == L"파란 타일 벽_Left.X" ||
				pObj->Get_Name() == L"여름 파도 무늬 벽_Left.X" ||
				pObj->Get_Name() == L"발렌타인 초콜릿 벽_Left.X"
				)
			{
				pObj->Set_Dead(TRUE);
			}
		}
	}
}

void CScene::Set_Dead_CafeWall_R(E_LAYERTAG eLayerTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return;

	map<E_OBJTAG, vector<CGameObject*>>* pLayerMap = iter->second->Get_LayerMapObject();

	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObj : tPair.second)
		{
			if (pObj->Get_Name() == L"내츄럴 나무 벽_Right.X" ||
				pObj->Get_Name() == L"패턴 타일 벽_Right.X" ||
				pObj->Get_Name() == L"파란 타일 벽_Right.X" ||
				pObj->Get_Name() == L"여름 파도 무늬 벽_Right.X" ||
				pObj->Get_Name() == L"발렌타인 초콜릿 벽_Right.X"
				)
			{
				pObj->Set_Dead(TRUE);
			}
		}
	}
}

HRESULT CScene::Ready_Scene()
{
	

	for (_int i = 0; i < (_int)E_LAYERTAG::TAG_END; ++i)
	{
		CLayer* pLayer = CLayer::Create();
		Add_Layer((E_LAYERTAG)i, pLayer);
	}
	m_pMainScene = this;

	

	return S_OK;
}

_int CScene::Start_Scene(const _float& fTimeDelta)
{
	m_bStart = FALSE; 

	CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_DYNAMIC);



	return RETURN_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		Start_Scene(fTimeDelta); 
	}

	_int iSize = (_int)m_vecNextFrame.size();
	for (_int i = 0; i < iSize; ++i)
	{
		Add_GameObject(m_vecNextFrame[i].first, m_vecNextFrame[i].second.first, m_vecNextFrame[i].second.second);
	}
	m_vecNextFrame.clear();

	_int iResult = 0;
 	for (auto& iter : m_mapLayer)
	{
		if (FALSE == iter.second->Get_Active())
			continue;

		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	
	return iResult;
}

_int CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iResult = 0;
	for (auto& iter : m_mapLayer)
	{
		if (FALSE == iter.second->Get_Active())
			continue;

		iResult = iter.second->LateUpdate_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::Set_FogEvent()
{
}

void CScene::Update_FogEvent(const _float & fTimeDelta)
{
}

void CScene::Active_AutoCam()
{
}

void CScene::Set_Fit_Camera()
{
}

void CScene::Set_Fit_Camera_Option(_float _fBaseY_At, _float _fBaseY_Eye, _float _fFarX, _float _fFarY, _float _fFarZ)
{
	m_fBaseY_At = _fBaseY_At;
	m_fBaseY_Eye = _fBaseY_Eye;
	m_fFarX = _fFarX;
	m_fFarY = _fFarY;
	m_fFarZ = _fFarZ;

	Set_Fit_Camera();
}

void CScene::Set_Fit_Camera_Interval(_float _fMinEye_X, _float _fMaxEye_X, _float _fMinEye_Y, _float _fMaxEye_Y, _float _fMinEye_Z, _float _fMaxEye_Z)
{
	m_fMinEye_X = _fMinEye_X;
	m_fMaxEye_X = _fMaxEye_X;
	m_fMinEye_Y = _fMinEye_Y;
	m_fMaxEye_Y = _fMaxEye_Y;
	m_fMinEye_Z = _fMinEye_Z;
	m_fMaxEye_Z = _fMaxEye_Z;

	Set_Fit_Camera();
}

void CScene::Set_SkySphere(wstring wstrSkyTexFullPath)
{
	if (!m_pSkySphere)
	{
		m_pSkySphere = CSkyDome::Create(m_pGraphicDev, wstrSkyTexFullPath);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_BackGround, m_pSkySphere);
	}
	else
	{
		static_cast<CSkyDome*>(m_pSkySphere)->Set_SkyTex(wstrSkyTexFullPath);
	}
}

void CScene::Update_Fit_Camera(const _float& fTimeDelta)
{
}

void CScene::Active_ShakeCam(const _float & fShakeTime, const _float & fShakePower)
{
}

void CScene::Update_ShakeCam(const _float & fTimeDelta)
{
}

_bool CScene::IsExist_Obj(const E_LAYERTAG & eLayerTag, const E_OBJTAG & eObjTag)
{
	auto	iter = m_mapLayer.find(eLayerTag);

	if (iter == m_mapLayer.end())
		return false;

	return iter->second->IsExist_Obj(eObjTag);
}

HRESULT CScene::Ready_SceneObject()
{
	return S_OK;
}

void CScene::Set_ClickEffect()
{
	if (!m_bKeyDown && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		_float fCircleZRot = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);
		POINT pt = CUtility::Get_MousePoint(*CUI_Manager::GetInstance()->Get_Hwnd());
		CUtility::Print_Wstring_DMode(to_wstring(pt.x) + L".,,, " + to_wstring(pt.y));
		CSkill_Manager::GetInstance()->SetUISkill(L"MouseClick.skill", E_LAYERTAG::Effect, _vec2(_float(pt.x), _float(WINCY - pt.y)), 0.f, 0.025f);


		/*for (_int i = 0; i < 5; ++i)
		{
			_float fScale = CWall512::GetInstance()->GetFloatValue(0.004f, 0.006f);
			_float fRot = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);
			_float fDistance = CWall512::GetInstance()->GetFloatValue(0.1f, 1.f);
			_float fSpeed = CWall512::GetInstance()->GetFloatValue(0.5f, 1.f);
			_vec3 vLook = VEC_UP;
			_matrix matRotZ;

			D3DXMatrixRotationZ(&matRotZ, fRot * Deg2Rad);
			D3DXVec3TransformNormal(&vLook, &vLook, &matRotZ);
			vLook.z = 0.f;
			
			_vec2 vUILook = _vec2(vLook.x, vLook.y);
			D3DXVec2Normalize(&vUILook, &vUILook);

			_float fZRot = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);


			CUISkillObject* pClickPart = CSkill_Manager::GetInstance()->SetUISkill(L"MouseClickTriangle.skill", E_LAYERTAG::Effect, _vec2(_float(pt.x), _float(WINCY - (pt.y))) + vUILook * 30.f * fDistance, fZRot, fScale);
			pClickPart->Set_Speed(50.f * fSpeed);
			pClickPart->Set_UIDir(vUILook);
			pClickPart->Set_RotSpeed(70.f * fSpeed);

		}*/

		if (!m_pMouseDragObj)
		{
			m_pMouseDragObj = CSkill_Manager::GetInstance()->SetUISkill(L"MouseDrag.skill", E_LAYERTAG::Effect, _vec2(_float(pt.x), _float(WINCY - (pt.y))), 0.f, 0.05f);
		}

		m_bKeyDown = TRUE;
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_pMouseDragObj)
		{
			POINT pt = CUtility::Get_MousePoint(*CUI_Manager::GetInstance()->Get_Hwnd());
			//마우스 드래그
			m_pMouseDragObj->Set_UIPos(_vec2(_float(pt.x), _float(WINCY - (pt.y))));
		}
	}
	if (m_bKeyDown && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		if (m_pMouseDragObj)
		{
			m_pMouseDragObj->Set_Dead(TRUE);
			m_pMouseDragObj = nullptr;
		}
		m_bKeyDown = FALSE;
	}

}

#ifdef _DEBUG
void CScene::Debugging_Console()
{
	_int iMapCount = (_int)m_mapLayer.size();
	cout << "--------   Scene   ---------" << endl;
	cout << "현재 씬에있는 Layer 개수 : " << iMapCount << "개" << endl;

	for (auto& iter : m_mapLayer)
	{
		iter.second->Debugging_Console();
	}
}

#endif

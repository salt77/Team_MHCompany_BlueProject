// (5인 팀) 블루아카이브 모작에서의 컷신용 이벤트 카메라 담당 코드
#include "ActionCam_Mgr.h"

#include "CamControl_Eye.h"
#include "CamControl_At.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CActionCam_Mgr)

CActionCam_Mgr::CActionCam_Mgr()
{
}

CActionCam_Mgr::~CActionCam_Mgr()
{
	Free();
}

void CActionCam_Mgr::Free()
{
	Clear_MapActionCam();
}

HRESULT CActionCam_Mgr::Ready_ActionCamManager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;

	return S_OK;
}

// 시간 값과 이벤트 카메라의 이동 경로를 갱신 
_int CActionCam_Mgr::Update_ActionCamManager(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta; 

	MoveOn_Line(); 

	return RETURN_OK;
}

// 이벤트 카메라 로드 함수(개별적으로 하고 싶을 때만 사용)
HRESULT CActionCam_Mgr::Load_ActionCam(const wstring& wstrFileName)
{
	m_pScene = CScene::Get_MainScene();

	wstring		wstrName = L"../../Reference/Data/Data_CamTest/" + wstrFileName;
	HANDLE	hFile = CreateFile(wstrName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwbyte = 0;

	_int	iActionCamEyeSize = -1;
	_int	iActionCamAtSize = -1;
	_int	iSectionSpeedSize = -1;

	_int	iVectorEyeSize = -1;
	_int	iVectorAtSize = -1;
	_int	iVectorSecSpeedSize = -1;

	_float	fSectionSpeed = -1.f;

	_vec3	vPos = VEC_ZERO;

	wstring wstrKey = L"";

	// 1. map의 사이즈 
	CUtility::Data_ReadInt(hFile, iActionCamEyeSize, dwbyte);
	CUtility::Data_ReadInt(hFile, iActionCamAtSize, dwbyte);
	CUtility::Data_ReadInt(hFile, iSectionSpeedSize, dwbyte);

	// 2. map 사이즈만큼 순회하면서 Key값과 Value의 사이즈 
	for (_int i = 0; i < iActionCamEyeSize; ++i)
	{
		CUtility::Data_ReadWString(hFile, wstrKey, dwbyte);
		CUtility::Data_ReadInt(hFile, iVectorEyeSize, dwbyte);

		for (_int j = 0; j < iVectorEyeSize; ++j)
		{
			_int iObjTag = (_int)E_OBJTAG::OBJ_END;
			CUtility::Data_WriteInt(hFile, iObjTag, dwbyte);

			CGameObject* pGameObject = CCamControl_Eye::Create(
				m_pGraphicDev,
				(E_OBJTAG)iObjTag,
				VEC_ZERO,
				1.5f);
			assert(pGameObject);
			ReadFile(hFile, &vPos, sizeof(_vec3), &dwbyte, nullptr);
			pGameObject->Set_Pos(vPos);
			m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject((E_OBJTAG)iObjTag, pGameObject);

			m_dbMapActionEye[wstrFileName][wstrKey].emplace_back(static_cast<CCamControl_Eye*>(pGameObject));
		}
	}

	for (_int i = 0; i < iActionCamAtSize; ++i)
	{
		CUtility::Data_ReadWString(hFile, wstrKey, dwbyte);
		CUtility::Data_ReadInt(hFile, iVectorAtSize, dwbyte);

		for (_int j = 0; j < iVectorAtSize; ++j)
		{
			_int iObjTag = (_int)E_OBJTAG::OBJ_END;
			CUtility::Data_WriteInt(hFile, iObjTag, dwbyte);

			CGameObject* pGameObject = CCamControl_At::Create(
				m_pGraphicDev,
				(E_OBJTAG)iObjTag,
				VEC_ZERO,
				1.5f);
			assert(pGameObject);
			ReadFile(hFile, &vPos, sizeof(_vec3), &dwbyte, nullptr);
			pGameObject->Set_Pos(vPos);
			m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject((E_OBJTAG)iObjTag, pGameObject);

			m_dbMapActionAt[wstrFileName][wstrKey].emplace_back(static_cast<CCamControl_At*>(pGameObject));
		}
	}

	for (_int i = 0; i < iSectionSpeedSize; ++i)
	{
		CUtility::Data_ReadWString(hFile, wstrKey, dwbyte);
		CUtility::Data_ReadInt(hFile, iVectorSecSpeedSize, dwbyte);

		m_dbMapSectionSpeed[wstrFileName][wstrKey].reserve(iVectorSecSpeedSize);

		for (_int j = 0; j < iVectorSecSpeedSize; ++j)
		{
			CUtility::Data_ReadFloat(hFile, fSectionSpeed, dwbyte);

			m_dbMapSectionSpeed[wstrFileName][wstrKey].emplace_back(fSectionSpeed);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

// 이벤트 카메라 시작(따로 로드하지 않았다면 자동으로 이벤트 데이터 로드 후 시작)
void CActionCam_Mgr::Start_ActionCam(const wstring& wstrFileName, const wstring& wstrAction, _vec3 vPos, _float fYAngle, E_ActionCam_Mode eMode)
{
	if (m_dbMapActionEye.end() == m_dbMapActionEye.find(wstrFileName))
	{
		Load_ActionCam(wstrFileName);
	}

	_bool		bExistKey = false;
	map<const wstring, vector<CCamControl_Eye*>>::iterator	iter_Eye = m_dbMapActionEye[wstrFileName].begin();

	for (; iter_Eye != m_dbMapActionEye[wstrFileName].end(); ++iter_Eye)
	{
		if (wstrAction == iter_Eye->first)
		{
			bExistKey = true; 
		}
	}

	// 로드할 데이터가 존재하지 않으면 코드 실행치 않고 함수 리턴 
	if (!bExistKey)
		return; 

	m_fAngle_Rot_Y = fYAngle;
	m_vAddPos = vPos;

	// 호출과 동시에 첫 번째 인덱스로 이동 
	if (!m_dbMapActionEye[wstrFileName].empty() &&
		!m_dbMapActionAt[wstrFileName].empty())
	{
		m_eCurActionMode = eMode; 
		m_wstrCurFile = wstrFileName;
		m_wstrCurEvent = wstrAction; 

		m_fTime = 0.f;
		m_iCurLineIndex = 1;
		m_fProgressCam = 0.f;

		m_pMainCam = static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera, 0));
		assert(m_pMainCam);
		m_pMainCam->Set_Mode(CCamera::CAM_ACTION);

		_vec3 vFinalPos = Get_Adjust_CamPos(m_dbMapActionEye[m_wstrCurFile][wstrAction][0]->Get_Info(E_INFO::INFO_POS));
		switch (m_eCurActionMode)
		{
		case CActionCam_Mgr::ActionCam_Subject:
			m_pMainCam->Set_CamEye(vFinalPos);
			break;

		case CActionCam_Mgr::ActionCam_Observe:
			m_pMainCam->Set_CamAt(vFinalPos);
			break;

		case CActionCam_Mgr::ActionCam_Normal:
			break;

		case CActionCam_Mgr::ActionCam_End:
			break;

		default:
			assert(0);
			break;
		}
	}
}

// 로드하지 않고 시작, 로드한 데이터가 없다면 코드 실행치 않고 함수 종료 
void CActionCam_Mgr::Start_ActionCam_NotLoad(const wstring & wstrFileName, const wstring & wstrAction, _vec3 vPos, _float fYAngle, E_ActionCam_Mode eMode)
{
	_bool		bExistKey = false;
	map<const wstring, vector<CCamControl_Eye*>>::iterator	iter_Eye = m_dbMapActionEye[wstrFileName].begin();

	for (; iter_Eye != m_dbMapActionEye[wstrFileName].end(); ++iter_Eye)
	{
		if (wstrAction == iter_Eye->first)
		{
			bExistKey = true;
		}
	}

	// 로드된 데이터 없을 시 코드 실행치 않고 함수 리턴  
	if (!bExistKey)
		return;

	m_fAngle_Rot_Y = fYAngle;
	m_vAddPos = vPos;

	// 호출과 동시에 첫 번째 인덱스로 이동 
	if (!m_dbMapActionEye[wstrFileName].empty() &&
		!m_dbMapActionAt[wstrFileName].empty())
	{
		m_eCurActionMode = eMode;
		m_wstrCurFile = wstrFileName;
		m_wstrCurEvent = wstrAction;

		m_fTime = 0.f;
		m_iCurLineIndex = 1;
		m_fProgressCam = 0.f;

		m_pMainCam = static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera, 0));
		assert(m_pMainCam);
		m_pMainCam->Set_Mode(CCamera::CAM_ACTION);

		_vec3 vFinalPos = Get_Adjust_CamPos(m_dbMapActionEye[m_wstrCurFile][wstrAction][0]->Get_Info(E_INFO::INFO_POS));
		switch (m_eCurActionMode)
		{
		case CActionCam_Mgr::ActionCam_Subject:
			m_pMainCam->Set_CamEye(vFinalPos);
			break;

		case CActionCam_Mgr::ActionCam_Observe:
			m_pMainCam->Set_CamAt(vFinalPos);
			break;

		case CActionCam_Mgr::ActionCam_Normal:
			break;

		case CActionCam_Mgr::ActionCam_End:
			break;

		default:
			assert(0);
			break;
		}
	}
}

// 이벤트 카메라 종료 시 초기화 코드 (별개로 실행할 필요없으나 필요 시 사용해도 됨)
void CActionCam_Mgr::Stop_ActionCam()
{
	m_eCurActionMode = ActionCam_Normal; 
	m_fTime = 0.f; 
	m_iCurLineIndex = 1;
	m_fProgressCam = 0.f;

	if (m_pMainCam)
	{
		m_pMainCam->Set_Mode(CCamera::CAM_FREE);
		m_pMainCam->Set_CamOriginSettings(); 
	}
}

// 소멸자와 함께 호출 
void CActionCam_Mgr::Clear_MapActionCam()
{
	if (!m_dbMapActionEye.empty())
	{
		m_dbMapSectionSpeed.clear();

		m_dbMapActionEye.clear();
		m_dbMapActionAt.clear();
	}
}

// 이벤트 카메라 이동 코드 
void CActionCam_Mgr::MoveOn_Line()
{
	if (ActionCam_Observe == m_eCurActionMode || ActionCam_Subject == m_eCurActionMode)
	{
		m_fProgressCam = CUtility::Clamp_float(m_fProgressCam, 0.f, 1.f);

		// 종료 코드 
		if (m_iCurLineIndex == m_dbMapActionEye[m_wstrCurFile][m_wstrCurEvent].size() - 3 &&
			m_fProgressCam >= 1.f)
		{
			m_eCurActionMode = ActionCam_Normal;
			m_pMainCam->Set_Mode(CCamera::CAM_FREE);

			return;
		}

		Eye_Line();
		At_Line();

		if (1.f <= m_fProgressCam)
		{
			m_fProgressCam = 0.f;
			m_fTime = 0.f;

			if ((_uint)m_dbMapActionEye[m_wstrCurFile][m_wstrCurEvent].size() - 3 > m_iCurLineIndex)
			{
				++m_iCurLineIndex;
			}
			else
			{
				m_eCurActionMode = ActionCam_Normal;
			}
		}

		m_fTime += m_fTimeDelta;
		m_fProgressCam = m_fTime / m_dbMapSectionSpeed[m_wstrCurFile][m_wstrCurEvent][m_iCurLineIndex]; 
	}
}

// 카메라 Eye 이동 코드 
void CActionCam_Mgr::Eye_Line()
{
	vector<CCamControl_Eye*>	vecPoint = m_dbMapActionEye[m_wstrCurFile][m_wstrCurEvent];

	if (3 < vecPoint.size())
	{
		if (0 < m_iCurLineIndex &&
			m_iCurLineIndex < vecPoint.size() - 2)
		{
			Make_Spline(&m_vLinePos,
						Get_Adjust_CamPos(vecPoint[m_iCurLineIndex - 1]->Get_Info(E_INFO::INFO_POS)),
						Get_Adjust_CamPos(vecPoint[m_iCurLineIndex]->Get_Info(E_INFO::INFO_POS)),
						Get_Adjust_CamPos(vecPoint[m_iCurLineIndex + 1]->Get_Info(E_INFO::INFO_POS)),
						Get_Adjust_CamPos(vecPoint[m_iCurLineIndex + 2]->Get_Info(E_INFO::INFO_POS)));
		}

		switch (m_eCurActionMode)
		{
		case E_ActionCam_Mode::ActionCam_Normal:
			break;

		case E_ActionCam_Mode::ActionCam_Observe:
			m_pMainCam->Set_CamAt(m_vLinePos);
			break;

		case E_ActionCam_Mode::ActionCam_Subject:
			m_pMainCam->Set_CamEye(m_vLinePos);
			break;

		default:
			assert(0);
			break;
		}
	}
}

// 카메라 At 이동 코드 
void CActionCam_Mgr::At_Line()
{
	vector<CCamControl_At*>		vecPoint = m_dbMapActionAt[m_wstrCurFile][m_wstrCurEvent];

	if (3 < vecPoint.size())
	{
		if (0 < m_iCurLineIndex &&
			m_iCurLineIndex < vecPoint.size() - 2)
		{
			Make_Spline(&m_vLineAtPos,
				Get_Adjust_CamPos(vecPoint[m_iCurLineIndex - 1]->Get_Info(E_INFO::INFO_POS)),
				Get_Adjust_CamPos(vecPoint[m_iCurLineIndex]->Get_Info(E_INFO::INFO_POS)),
				Get_Adjust_CamPos(vecPoint[m_iCurLineIndex + 1]->Get_Info(E_INFO::INFO_POS)),
				Get_Adjust_CamPos(vecPoint[m_iCurLineIndex + 2]->Get_Info(E_INFO::INFO_POS)));

			if (E_ActionCam_Mode::ActionCam_Subject == m_eCurActionMode)
			{
				m_pMainCam->Set_CamAt(m_vLineAtPos);
			}
		}
	}

	if (1 == vecPoint.size())
	{
		if (E_ActionCam_Mode::ActionCam_Subject == m_eCurActionMode)
		{
			m_pMainCam->Set_CamAt(Get_Adjust_CamPos(vecPoint[0]->Get_Info(E_INFO::INFO_POS)));
		}
	}
}

// 캣멀롬 함수로 가중치 누적하여 곡선 보간 
void CActionCam_Mgr::Make_Spline(_vec3 * vOutPos, const _vec3 & vPos0, const _vec3 & vPos1, const _vec3 & vPos2, const _vec3 & vPos3)
{
	_float fWeight = m_fProgressCam / 1.f;

	D3DXVec3CatmullRom(vOutPos, &vPos0, &vPos1, &vPos2, &vPos3, fWeight);
}

void CActionCam_Mgr::Set_ClearAllPoint()
{
	vector<CGameObject*>*	vecObject_Eye = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_CamControl_Eye); 
	vector<CGameObject*>*	vecObject_At = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_CamControl_At);

	for (_uint i = 0; i < vecObject_Eye->size(); ++i)
	{
		(*vecObject_Eye)[i]->Set_Dead(TRUE);
	}
	for (_uint i = 0; i < vecObject_At->size(); ++i)
	{
		(*vecObject_At)[i]->Set_Dead(TRUE);
	}

	Clear_MapActionCam(); 
}

_vec3 CActionCam_Mgr::Get_Adjust_CamPos(_vec3 vOrigin)
{
	_vec3 vFinalPos = vOrigin;
	_matrix matRot;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationY(&matRot, m_fAngle_Rot_Y * Deg2Rad);
	D3DXVec3TransformCoord(&vFinalPos, &vFinalPos, &matRot);
	vFinalPos += m_vAddPos;

	return vFinalPos;
}

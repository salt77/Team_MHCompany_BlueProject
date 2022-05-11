#include "EffectObject.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "Wall512.h"
#include "Scene.h"
#include "PrefabManager.h"

USING(Engine)



CEffectObject::CEffectObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _vec3 vRot)
	: CGameObject(pGraphicDev,eObjTag)
{
	m_pShader = nullptr;
	m_pTexture = nullptr;
	m_pEMesh = nullptr;
	m_fTimeAcc = 0.f;
	m_fRatio = 0.f;
	m_iSpriteIndex = 0;
	m_fSpriteTime = 0.f;
	
	m_vPos = vPos;
	m_vRot = vRot;

	D3DXMatrixIdentity(&m_matRot); 
	D3DXMatrixIdentity(&m_matWorld); 
	D3DXQuaternionIdentity(&m_quatRot); 
}

CEffectObject::CEffectObject(const CEffectObject & rhs)
	:CGameObject(rhs)
{
	m_pShader = nullptr;
	m_pTexture = nullptr;
	m_pEMesh = nullptr;
	m_fTimeAcc = 0.f;
	m_fRatio = 0.f;
	m_iSpriteIndex = 0;
	m_fSpriteTime = 0.f;

	if (E_EffectRenderType::Buffer == m_tEffectInfo.eEffectRenderType)
	{
		m_bMesh = false;
	}
	else
	{
		m_bMesh = true;
	}
}

CEffectObject::~CEffectObject()
{
}

// 이펙트의 초기 설정 - with Struct
HRESULT CEffectObject::Ready_Object(T_EffectInfo tEffectInfo)
{
	CGameObject::Ready_Object();

	m_tEffectInfo = tEffectInfo;

	if (E_EffectRenderType::Buffer == m_tEffectInfo.eEffectRenderType)
	{
		m_bMesh = false;
	}
	else
	{
		m_bMesh = true;
	}

	Add_Component();

	Set_Pos(m_vPos + m_tEffectInfo.vPos_Start);
	Set_Rotation(m_vRot + m_tEffectInfo.vRot_Start);
	Set_Scale(m_tEffectInfo.vScale_Start);

	return S_OK;
}

// 이펙트의 초기 설정 - with Tag
HRESULT CEffectObject::Ready_Object(wstring wstrEffectTag)
{
	CGameObject::Ready_Object();

	PrefabLoad(wstrEffectTag);

	if (E_EffectRenderType::Buffer == m_tEffectInfo.eEffectRenderType)
	{
		m_bMesh = false;
	}
	else
	{
		m_bMesh = true;
	}

	Add_Component();

	Set_Pos(m_vPos + m_tEffectInfo.vPos_Start);
	Set_Rotation(m_vRot + m_tEffectInfo.vRot_Start);
	Set_Scale(m_tEffectInfo.vScale_Start);

	return S_OK;
}

HRESULT CEffectObject::Ready_Object(wstring wstrEffectTag, _bool bQuatRot)
{
	CGameObject::Ready_Object();

	PrefabLoad(wstrEffectTag);

	if (E_EffectRenderType::Buffer == m_tEffectInfo.eEffectRenderType)
	{
		m_bMesh = false;
	}
	else
	{
		m_bMesh = true;
	}

	Add_Component();

	Set_Pos(m_vPos + m_tEffectInfo.vPos_Start);
	Set_Scale(m_tEffectInfo.vScale_Start);

	m_bQuatRot = bQuatRot; 

	return S_OK;
}

// 오브젝트 스타트
_int CEffectObject::Start_Object(const _float & fTimeDelta)
{
	CGameObject::Start_Object(fTimeDelta);

	return RETURN_OK;
}

// 이펙트의 변위 등을 계산함
_int CEffectObject::Update_Object(const _float & fTimeDelta)
{
	if (!UpdateTime(fTimeDelta))
	{
		// 아직 이펙트의 딜레이 시간이다.
		return RETURN_OK;
	}
	
	Set_Ratio_EffectType();

	_vec3 vPos = m_tEffectInfo.vPos_Start + (m_tEffectInfo.vPos_End - m_tEffectInfo.vPos_Start) * m_fRatio;
	_vec3 vRot = m_tEffectInfo.vRot_Start + (m_tEffectInfo.vRot_End - m_tEffectInfo.vRot_Start) * m_fRatio;
	_vec3 vScale = m_tEffectInfo.vScale_Start + (m_tEffectInfo.vScale_End - m_tEffectInfo.vScale_Start) * m_fRatio;

	Set_Pos(vPos);
	if (!m_bQuatRot)
	{
		Set_Rotation(vRot);
	}	
	Set_Scale(vScale);
		
	if (m_tEffectInfo.bIsSprite) 
	{
		m_fSpriteTime += fTimeDelta * m_tEffectInfo.fSpeedCount;

		_float fTime = m_tEffectInfo.fLifeTime / (_float)(m_tEffectInfo.iRow * m_tEffectInfo.iColumn);

		if (m_fSpriteTime > fTime)
		{
			m_fSpriteTime -= fTime;
			
			++m_iSpriteIndex;
		}

		if (m_iSpriteIndex > m_tEffectInfo.iRow * m_tEffectInfo.iColumn) 
		{
			m_iSpriteIndex = 0;
		}
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	return iExit;
}

// 오브젝트 레이트업데이트
_int CEffectObject::LateUpdate_Object(const _float & fTimeDelta)
{
	if (m_bDead) 
	{
		return RETURN_OK;
	}

	CGameObject::LateUpdate_Object(fTimeDelta);
	if (!m_bShowWindow)
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_EFFECT, this);
	}
	else
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHOWWINDOW, this);
	}
	return RETURN_OK;
}

// 오브젝트 랜더 (메시 이펙트인지 아닌지에 따라 랜더가 달라짐)
void CEffectObject::Render_Object(_int iPass)
{
	if (m_bDead																		||
		(m_fTimeAcc <= m_tEffectInfo.fDelayTime || !m_pShader || !m_pTexture)		||
		(m_bMesh && !m_pEMesh)														||		
		(!m_bMesh && !m_pBuffer))
	{
		return;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// 0 알파옵션
	_int iCount_iTemp = 0;

	// 알파트랜스(0),알파블랜드(1),논알파 (2)의 값만 가짐
	if (0 == m_tEffectInfo.iTemp[iCount_iTemp])
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000010);
	}
	else if (1 == m_tEffectInfo.iTemp[iCount_iTemp])
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	else if (2 == m_tEffectInfo.iTemp[iCount_iTemp])
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	else
	{
		return;
	}

	// 1 컬옵션
	iCount_iTemp++;

	// 양면 표시(0), 후면 추려내기(1), 전면 추려내기 (2)의 값만 가짐
	if (0 == m_tEffectInfo.iTemp[iCount_iTemp])
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	else if (1 == m_tEffectInfo.iTemp[iCount_iTemp])
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	else if (2 == m_tEffectInfo.iTemp[iCount_iTemp])
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}
	else
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	_int iCount_bTemp = 0;

	// 0 블러옵션
	// 일반(0), 블러(1)
	_int iBlurPass = (_int)m_tEffectInfo.bTemp[iCount_bTemp++];

	// 1 빌보드 옵션
	// 일반(0), 빌보드(1)
	m_bBillboard = BOOL2bool(m_tEffectInfo.bTemp[iCount_bTemp++]);

	// 2 Zenable 옵션
	_bool bZ = m_tEffectInfo.bTemp[iCount_bTemp++];
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZENABLE, bZ);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, bZ);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Set_Constantable();

	m_pShader->BeginShader();
	m_pShader->BeginPass(iBlurPass);


	if (m_bMesh) 
	{
		m_pEMesh->Render_Meshes();
	}
	else
	{
		m_pBuffer->Render_Buffer();
	}
	

	m_pShader->EndPass();
	m_pShader->EndShader();
}

void CEffectObject::Free()
{
	CGameObject::Free();
}

// iCount가 2면 0도 180도로 1개씩 생성한다. 4면 0도 90도 180도 270도 로 생성한다.
// y각도를 원형으로 생성한다고 보면됨.
void CEffectObject::Create_Effect_RandAngle(LPDIRECT3DDEVICE9 pGraphicDev, _int iCount, _vec3 vPos, _vec3 vAngle, wstring wstrName)
{
	_float fAngleRand = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);
	for (_int i = 0; i < iCount; i++)
	{
		CEffectObject* pAura = CEffectObject::Create(pGraphicDev, E_OBJTAG::OBJ_Effect, vPos, VEC_ZERO, wstrName);
		pAura->Get_Effect_Info().vRot_Start.y += fAngleRand + (i / (_float)iCount * 360.f);
		pAura->Get_Effect_Info().vRot_End.y += fAngleRand + (i / (_float)iCount * 360.f);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pAura->Get_ObjTag(), pAura);
	}
}

_float CEffectObject::Saturate_Float(_float& fVar)
{
	if (0.f >= fVar)
	{
		fVar = 0.f;
	}
	else if (1.f <= fVar)
	{
		fVar = 1.f;
	}

	return fVar;
}

void CEffectObject::PrefabLoad(wstring wstrPrefabName)
{
	if (wstrPrefabName.size() < 4) 
	{
		return;
	}
	
	T_EffectInfo tEffectInfo = T_EffectInfo();
	
	DWORD dwByte;

	_int iSize = 0;
	_float fSize = 0;
	wchar_t wstrTex[MAX_PATH];

	// textPath
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, wstrTex, iSize, &dwByte);
	tEffectInfo.textPath = wstrTex;

	// eEffectRenderType
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	tEffectInfo.eEffectRenderType = (E_EffectRenderType)iSize;

	// meshPath
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, wstrTex, iSize, &dwByte);
	tEffectInfo.meshPath = wstrTex;

	// eEffectType
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	tEffectInfo.eEffectType = (E_EffectType)iSize;

	// fSpeedCount 
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fSpeedCount, sizeof(_float), &dwByte);
	// fDelayTime
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fDelayTime, sizeof(_float), &dwByte);
	// fLifeTime
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fLifeTime, sizeof(_float), &dwByte);
	
	// iPass
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.iPass, sizeof(_int), &dwByte);

	// fU_Start 
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fU_Start, sizeof(_float), &dwByte);
	// fU_End
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fU_End, sizeof(_float), &dwByte);
	// fV_Start
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fV_Start, sizeof(_float), &dwByte);
	// fV_End
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fV_End, sizeof(_float), &dwByte);

	// vPos_Start 
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vPos_Start, sizeof(_vec3), &dwByte);
	// vPos_End
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vPos_End, sizeof(_vec3), &dwByte);
	// vRot_Start 
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vRot_Start, sizeof(_vec3), &dwByte);
	// vRot_End
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vRot_End, sizeof(_vec3), &dwByte);
	// vScale_Start 
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vScale_Start, sizeof(_vec3), &dwByte);
	// vScale_End
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vScale_End, sizeof(_vec3), &dwByte);
	
	// vColor_Start 
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vColor_Start, sizeof(_vec4), &dwByte);
	// vColor_End
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.vColor_End, sizeof(_vec4), &dwByte);

	// bIsSprite
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.bIsSprite, sizeof(_bool), &dwByte);
	// iRow
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.iRow, sizeof(_int), &dwByte);
	// iPasiColumns
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.iColumn, sizeof(_int), &dwByte);

	// filterPath_1
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, wstrTex, iSize, &dwByte);
	tEffectInfo.filterPath_1 = wstrTex;
	// filterPath_2
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, wstrTex, iSize, &dwByte);
	tEffectInfo.filterPath_2 = wstrTex;
	// filterPath_3
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, wstrTex, iSize, &dwByte);
	tEffectInfo.filterPath_3 = wstrTex;

	// bBlackAlpha
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.bBlackAlpha, sizeof(_bool), &dwByte);
	// bBlackWhite
	CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.bBlackWhite, sizeof(_bool), &dwByte);

	// iTemp
	for (_int i = 0; i < 10; i++)
	{
		CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.iTemp[i], sizeof(_int), &dwByte);
	}

	// bTemp
	for (_int i = 0; i < 10; i++)
	{
		CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.bTemp[i], sizeof(_bool), &dwByte);
	}

	// fTemp

	// iTemp[9]를 이용하여 old new 검사 
	_int iNew = tEffectInfo.iTemp[9];
	if (9 == iNew)
	{
		// 기존 세이브 파일
		// 추가된 뒷 정보 "float[20]"을 읽어온다.
		for (_int i = 0; i < 20; i++)
		{
			CPrefabManager::GetInstance()-> ReadPrefab(wstrPrefabName, &tEffectInfo.fTemp[i], sizeof(_float), &dwByte);
		}
	}
	else
	{
		// 기본값 입력
	}

	CPrefabManager::GetInstance()-> FileReset();

	m_tEffectInfo = tEffectInfo;
}

void CEffectObject::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CEffectObject::Load(HANDLE hFile, DWORD & dwByte)
{
}

void CEffectObject::Add_Effect_Rotation(const E_ROTATION & eRotationType, const _float & fAngle)
{
	_quat		qRot;
	_matrix		matRot;

	D3DXQuaternionIdentity(&qRot);

	D3DXMatrixIdentity(&matRot);

	switch (eRotationType)
	{
	case Engine::E_ROTATION::ROT_X:
		D3DXQuaternionRotationAxis(&qRot, &m_vAxis[(_uint)E_ROTATION::ROT_X], D3DXToRadian(fAngle));
		m_quatRot = m_quatRot * qRot;

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Z], &m_vAxis[(_uint)E_ROTATION::ROT_Z], &matRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Y], &m_vAxis[(_uint)E_ROTATION::ROT_Y], &matRot);
		break;

	case Engine::E_ROTATION::ROT_Y:
		D3DXQuaternionRotationAxis(&qRot, &m_vAxis[(_uint)E_ROTATION::ROT_Y], D3DXToRadian(fAngle));
		m_quatRot = m_quatRot * qRot;

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_X], &m_vAxis[(_uint)E_ROTATION::ROT_X], &matRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Z], &m_vAxis[(_uint)E_ROTATION::ROT_Z], &matRot);
		break;

	case Engine::E_ROTATION::ROT_Z:
		D3DXQuaternionRotationAxis(&qRot, &m_vAxis[(_uint)E_ROTATION::ROT_Z], D3DXToRadian(fAngle));
		m_quatRot = m_quatRot * qRot;

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_X], &m_vAxis[(_uint)E_ROTATION::ROT_X], &matRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Y], &m_vAxis[(_uint)E_ROTATION::ROT_Y], &matRot);
		break;

	default:
		MSG_BOX("CRun_Player->Add_Rotation_Quat() : 회전축 입력 오류");
		break;
	}

	for (_uint i = 0; i < (_uint)E_ROTATION::ROT_END; ++i)
	{
		D3DXVec3Normalize(&m_vAxis[i], &m_vAxis[i]);
	}

	D3DXMatrixRotationQuaternion(&m_matRot, &m_quatRot);
}

// 오브젝트 에드 컴포넌트
void CEffectObject::Add_Component()
{
	CComponent*		pComponent = nullptr;

	if (m_bMesh)
	{
		// mesh
		m_pEMesh = static_cast<CEMesh*>(Emplace_Component(L"Com_EMesh", CEMesh::Create(m_pGraphicDev, m_tEffectInfo.meshPath)));
	}
	else
	{
		// buffer
		m_pBuffer = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));
	}

	// Shader
	m_pShader = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect")));

	// Texture
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_tEffectInfo.textPath, &m_pTexture);
		
	// Filter_1
	if (L"None" != m_tEffectInfo.filterPath_1)
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_tEffectInfo.filterPath_1, &m_pTexture_Filter_1);
	}

	// Filter_2
	if (L"None" != m_tEffectInfo.filterPath_2)
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_tEffectInfo.filterPath_2, &m_pTexture_Filter_2);
	}

	// Filter_3
	if (L"None" != m_tEffectInfo.filterPath_3)
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_tEffectInfo.filterPath_3, &m_pTexture_Filter_3);
	}
}

// 오브젝트 셋 콘스탄트 테이블
void CEffectObject::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	
	_int iPosAddX = 0;
	_int iPosAddY = 0;
	_float fScaleAddX = 1.f;
	_float fScaleAddY = 1.f;

	_float fRatio = m_fRatio;
	if (1.f <= fRatio)
	{
		fRatio = 1.f;
	}

	if (m_tEffectInfo.bIsSprite)
	{
		iPosAddX = m_iSpriteIndex % m_tEffectInfo.iColumn;
		iPosAddY = (m_iSpriteIndex - iPosAddX) / m_tEffectInfo.iColumn;
		fScaleAddX = 1.f / (_float)m_tEffectInfo.iColumn;
		fScaleAddY = 1.f / (_float)m_tEffectInfo.iRow;

		pEffect->SetFloat("g_fU", (_float)iPosAddX);
		pEffect->SetFloat("g_fV", (_float)iPosAddY);
	}
	else
	{
		pEffect->SetFloat("g_fU", m_tEffectInfo.fU_Start + (m_tEffectInfo.fU_End - m_tEffectInfo.fU_Start) * fRatio);
		pEffect->SetFloat("g_fV", m_tEffectInfo.fV_Start + (m_tEffectInfo.fV_End - m_tEffectInfo.fV_Start) * fRatio);
	}

	pEffect->SetFloat("g_fU_Scale", fScaleAddX);
	pEffect->SetFloat("g_fV_Scale", fScaleAddY);

	//////////////////////////////////////////////////////////////////////////////////////////
	// filter 에 들어가는 라티오 계산 , 모든 값의 단위는 라티오이다.

	_float fFilterBegin, fFilterEnd, fNumerator, fDenominator;

	//////////////////////////////////////////////////////////////////////////////////////////

	_float fRatio_1 = fRatio;
	if (m_tEffectInfo.bTemp[3]) 
	{
		fRatio_1 = 1.f - fRatio_1;
	}
	Saturate_Float(fRatio_1);

	fFilterBegin	= Saturate_Float(m_tEffectInfo.fTemp[0]);
	fFilterEnd		= Saturate_Float(m_tEffectInfo.fTemp[1]);
		
	fNumerator = 0.f;
	if (fFilterBegin <= fRatio_1)
	{
		fNumerator = fRatio_1 - fFilterBegin;
	}

	fDenominator = 1.f;
	if (fFilterBegin < fFilterEnd)
	{
		fDenominator = fFilterEnd - fFilterBegin;
	}
	
	Saturate_Float(fNumerator);
	Saturate_Float(fDenominator);

	pEffect->SetFloat("g_fRatio_1", fNumerator / fDenominator);

	//////////////////////////////////////////////////////////////////////////////////////////

	_float fRatio_2 = fRatio;
	if (m_tEffectInfo.bTemp[4])
	{
		fRatio_2 = 1.f - fRatio_2;
	}
	Saturate_Float(fRatio_2);

	fFilterBegin = Saturate_Float(m_tEffectInfo.fTemp[13]);
	fFilterEnd = Saturate_Float(m_tEffectInfo.fTemp[14]);

	fNumerator = 0.f;
	if (fFilterBegin <= fRatio_2)
	{
		fNumerator = fRatio_2 - fFilterBegin;
	}

	fDenominator = 1.f;
	if (fFilterBegin < fFilterEnd)
	{
		fDenominator = fFilterEnd - fFilterBegin;
	}

	Saturate_Float(fNumerator);
	Saturate_Float(fDenominator);

	pEffect->SetFloat("g_fRatio_2", fNumerator / fDenominator);

	//////////////////////////////////////////////////////////////////////////////////////////

	_int iCount_fTemp = 2;
	pEffect->SetFloat("g_fMin_Filter_1", Saturate_Float(m_tEffectInfo.fTemp[iCount_fTemp++]));
	pEffect->SetFloat("g_fMax_Filter_1", Saturate_Float(m_tEffectInfo.fTemp[iCount_fTemp++]));

	pEffect->SetFloat("g_fR_Filter_1", m_tEffectInfo.fTemp[iCount_fTemp++]);
	pEffect->SetFloat("g_fG_Filter_1", m_tEffectInfo.fTemp[iCount_fTemp++]);
	pEffect->SetFloat("g_fB_Filter_1", m_tEffectInfo.fTemp[iCount_fTemp++]);
	
	float fVar = 0.f;
	fVar = m_tEffectInfo.fTemp[iCount_fTemp++];
	pEffect->SetFloat("g_fR_Filter_4", fVar);
	fVar = m_tEffectInfo.fTemp[iCount_fTemp++];
	pEffect->SetFloat("g_fG_Filter_4", fVar);
	fVar = m_tEffectInfo.fTemp[iCount_fTemp++];
	pEffect->SetFloat("g_fB_Filter_4", fVar);
	fVar = m_tEffectInfo.fTemp[iCount_fTemp++];
	pEffect->SetFloat("g_fA_Filter_4", fVar);
	fVar = m_tEffectInfo.fTemp[iCount_fTemp++];
	pEffect->SetFloat("g_fU_Filter_4", fVar);
	fVar = m_tEffectInfo.fTemp[iCount_fTemp++];
	pEffect->SetFloat("g_fV_Filter_4", fVar);

	iCount_fTemp = 15;
	pEffect->SetFloat("g_fMin_Filter_2", Saturate_Float(m_tEffectInfo.fTemp[iCount_fTemp++]));
	pEffect->SetFloat("g_fMax_Filter_2", Saturate_Float(m_tEffectInfo.fTemp[iCount_fTemp++]));

	pEffect->SetFloat("g_fR_Filter_2", m_tEffectInfo.fTemp[iCount_fTemp++]);
	pEffect->SetFloat("g_fG_Filter_2", m_tEffectInfo.fTemp[iCount_fTemp++]);
	pEffect->SetFloat("g_fB_Filter_2", m_tEffectInfo.fTemp[iCount_fTemp++]);

	//////////////////////////////////////////////////////////////////////////////////////////

	_matrix	matView, matInverView, matProj, matParent;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//0204mat
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	// 빌보드
	if (m_bBillboard) 
	{
		// 뷰행렬의 역행렬
		matInverView = matView;
		matInverView._41 = 0;
		matInverView._42 = 0;
		matInverView._43 = 0;

		D3DXMatrixInverse(&matInverView, NULL, &matInverView);

		// 부모의 이동, 회전
		_matrix matParentTrans, matParentRot, matParentScale;
		D3DXMatrixTranslation(&matParentTrans, m_vPos.x, m_vPos.y, m_vPos.z);
		D3DXMatrixScaling(&matParentScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixIdentity(&matParentRot);
		
		matParent = matParentScale * matParentRot * matParentTrans;
	}
	// 빌보드가 아니면
	else
	{
		// 뷰행렬의 역행렬
		D3DXMatrixIdentity(&matInverView);

		// 부모의 이동, 회전
		_matrix matParentTrans, matParentRotX, matParentRotY, matParentRotZ, matParentScale;
		D3DXMatrixTranslation(&matParentTrans, m_vPos.x, m_vPos.y, m_vPos.z);
		D3DXMatrixScaling(&matParentScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixRotationX(&matParentRotX, m_vRot.x * Deg2Rad);
		D3DXMatrixRotationY(&matParentRotY, m_vRot.y * Deg2Rad);
		D3DXMatrixRotationZ(&matParentRotZ, m_vRot.z * Deg2Rad);

		matParent = matParentScale * matParentRotX * matParentRotY * matParentRotZ * matParentTrans;
	}

	if (!m_bQuatRot) 
	{
		pEffect->SetMatrix("g_matWorld", &(Get_Transform()->Get_WorldMatrix() * matInverView * matParent));
	}
	else 
	{
		_matrix		matScale, matTrans; 
		D3DXMatrixIdentity(&matScale); 
		D3DXMatrixIdentity(&matTrans); 

		D3DXMatrixScaling(&matScale, Get_Scale().x, Get_Scale().y, Get_Scale().z); 
		D3DXMatrixTranslation(&matTrans, Get_Position().x, Get_Position().y, Get_Position().z); 

		m_matWorld = matScale * m_matRot * matTrans; 

		pEffect->SetMatrix("g_matWorld", &(m_matWorld * matInverView * matParent)); 
	}
	//0204mat
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);	

	//////////////////////////////////////////////////////////////////////////////////////////

	_vec4 vColor = m_tEffectInfo.vColor_Start + (m_tEffectInfo.vColor_End - m_tEffectInfo.vColor_Start) * fRatio;
	pEffect->SetVector("g_vColor", &vColor);
	
	if (m_pTexture)
	{
		pEffect->SetTexture("g_BaseTexture", m_pTexture);
	}

	pEffect->SetBool("g_bFilter_1", BOOL2bool(m_pTexture_Filter_1));
	pEffect->SetBool("g_bFilter_2", BOOL2bool(m_pTexture_Filter_2));
	pEffect->SetBool("g_bFilter_3", BOOL2bool(m_pTexture_Filter_3));

	if (m_pTexture_Filter_1)
	{
		pEffect->SetTexture("g_FilterTexture_1", m_pTexture_Filter_1);
	}
	if (m_pTexture_Filter_2)
	{
		pEffect->SetTexture("g_FilterTexture_2", m_pTexture_Filter_2);
	}
	if (m_pTexture_Filter_3)
	{
		pEffect->SetTexture("g_FilterTexture_3", m_pTexture_Filter_3);
	}

	pEffect->SetBool("g_bBlackAlpha", m_tEffectInfo.bBlackAlpha);
	pEffect->SetBool("g_bBlackWhite", m_tEffectInfo.bBlackWhite);

	//////////////////////////////////////////////////////////////////////////////////////////

}

void CEffectObject::Debug_This_In_Console()
{
}

// float로 이펙트 타입에 따라 ratio를 반환한다
void CEffectObject::Set_Ratio_EffectType()
{
	_float fRealTime = m_fTimeAcc - m_tEffectInfo.fDelayTime;
	_float fRatio = fRealTime / m_tEffectInfo.fLifeTime;
	
	switch (m_tEffectInfo.eEffectType)
	{
	case Engine::E_EffectType::Once:
		// 속도
		fRatio *= m_tEffectInfo.fSpeedCount;
		break;

	case Engine::E_EffectType::Loop:
		// 횟수
		fRatio *= m_tEffectInfo.fSpeedCount;
		while (1.f < fRatio)
		{
			fRatio -= 1.f;
		}
		break;

	case Engine::E_EffectType::Once_PingPong:
		// 속도
		if (0.5f > fRatio)
		{
			fRatio = fRatio * m_tEffectInfo.fSpeedCount * 2.f;
		}
		else
		{
			fRatio = (1.f - fRatio) * m_tEffectInfo.fSpeedCount * 2.f;
		}

		if (1.f <= fRatio)
		{
			fRatio = 1.f;
		}
		break;

	case Engine::E_EffectType::Loop_PingPong:

		// 횟수		
		fRatio = fRatio * m_tEffectInfo.fSpeedCount;

		while (1.f <= fRatio)
		{
			fRatio -= 1.f;
		}
		
		if (0.5f > fRatio)
		{
		}
		else
		{
			fRatio = 1.f - fRatio;
		}
		break;

	default:
		// 있을 수 없는 타입
		assert(FALSE);
		break;
	}

	m_fRatio = fRatio;
}

void CEffectObject::Set_Axis(const E_ROTATION & eRotationType, const _vec3& vDir)
{
	m_vAxis[(_uint)eRotationType] = *D3DXVec3Normalize(&m_vAxis[(_uint)eRotationType], &vDir);
}

// 이펙트의 지연 시간, 지속 시간등을 계산 
_bool CEffectObject::UpdateTime(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc <= m_tEffectInfo.fDelayTime)
	{
		// 이펙트의 대기 시간
		return false;
	}

	if (m_fTimeAcc >= m_tEffectInfo.fDelayTime + m_tEffectInfo.fLifeTime)
	{
		m_bDead = true;
		return false;
	}
	
	return true;
}

CEffectObject * CEffectObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, T_EffectInfo tEffectInfo)
{
	CEffectObject* pInstance = new CEffectObject(pGraphicDev, eObjTag, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f));
	
	if (FAILED(pInstance->Ready_Object(tEffectInfo)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffectObject * CEffectObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _vec3 vRot, wstring wstrEffectTag)
{
	CEffectObject* pInstance = new CEffectObject(pGraphicDev, eObjTag, vPos, vRot);

	if (FAILED(pInstance->Ready_Object(wstrEffectTag)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffectObject * CEffectObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _vec3 vRot, wstring wstrEffectTag, _bool bQuatRot)
{
	CEffectObject* pInstance = new CEffectObject(pGraphicDev, eObjTag, vPos, vRot);

	if (FAILED(pInstance->Ready_Object(wstrEffectTag)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}


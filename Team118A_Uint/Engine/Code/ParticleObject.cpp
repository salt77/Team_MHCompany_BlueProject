#include "ParticleObject.h"
#include "ProtoMgr.h"
#include "Wall512.h"
#include "Renderer.h"
#include "CrossRectBuffer.h"
#include "PrefabManager.h"
#include "RectBuffer.h"
#include "Scene.h"
USING(Engine)

CParticleObject::CParticleObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CGameObject(pGraphicDev,eObjTag)
{
}


CParticleObject::CParticleObject(const CParticleObject & rhs)
	:CGameObject(rhs)
	,m_pDecl(rhs.m_pDecl)
	,m_iVertexSize(rhs.m_iVertexSize)
{

}

CParticleObject::~CParticleObject()
{
}


//기본적인 준비작업을 수행하는 함수
HRESULT CParticleObject::Ready_Object(wstring wstrParticleTag)
{
	CGameObject::Ready_Object();
	m_wstrParticleTag = wstrParticleTag;
	Add_Component();

	m_pGraphicDev->CreateVertexDeclaration(declRect, &m_pDecl);

	m_fStartTime = CScene::Get_PlayTime();

	PrefabLoad(wstrParticleTag);


	return S_OK;
}

//깡통
_int CParticleObject::Start_Object(const _float & fTimeDelta)
{
	CGameObject::Start_Object(fTimeDelta);
	return RETURN_OK;
}

//파티클 오브젝트의 업데이트를 수행하는 함수
_int CParticleObject::Update_Object(const _float & fTimeDelta)
{
	//m_pTransformCom->Add_Rotation(E_ROTATION::ROT_Y, 10.f*fTimeDelta);

	
	if (m_bDead)
	{
		return RETURN_DEAD;
	}
	if (m_fStartTime + m_fLifeTime < CScene:: Get_PlayTime())
	{
		m_bDead = true;
	}

	if (m_listWaitSpawner.empty() && m_listActiveSpawner.empty())
	{
		_bool bCheck = TRUE;
		for (auto& listParticle : m_vecListParticle)
		{
			if (!listParticle.empty())
			{
				bCheck = FALSE;
				break;
			}
		}
		if (bCheck)
		{
			m_bDead = TRUE;
		}
	}

	CGameObject::Update_Object(fTimeDelta);

	WaitSpawnerCheck();
	SpawnerUpdate(fTimeDelta);
	ParticleUpdate(fTimeDelta);
	SetInstanceBuffer();

	return RETURN_OK;
}

//렌더 그룹에 추가해주는 함수
_int CParticleObject::LateUpdate_Object(const _float & fTimeDelta)
{
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

//인스턴싱을 통해 파티클을 렌더링하는 함수
void CParticleObject::Render_Object(_int iPass)
{
	if (m_vecTexture.empty())
	{
		return;
	}

	m_pShaderCom->BeginShader();
	for (_int i = 0; i < _int(m_vecTexture.size()); ++i)
	{
	
		if (m_vecListParticle[i].empty())
		{
			continue;
		}

		switch (m_vecListParticle[i].front().iAlphaOption)
		{
		case 0: //알파 트랜스
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000010);
			break;
		case 1: //알파 블렌드
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			break;
		case 2: //논 알파

			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			break;
		default:
			break;
		}

		// 0 블러옵션
		// 일반(0), 블러(1)
		_int iBlurPass = (_int)m_vecListParticle[i].front().bBlur;


		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZENABLE, m_vecListParticle[i].front().bZEnable);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, m_vecListParticle[i].front().bZEnable);

		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
		m_pShaderCom->BeginPass(2+iBlurPass);
		//======= 그리기 ======

		m_pGraphicDev->SetVertexDeclaration(m_pDecl);

		LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
		if (m_vecListParticle[i].front().bUseCross)
		{
			pVertexBuffer = m_pCrossRectBuffer->Get_VB();
		}
		else
		{
			pVertexBuffer = m_pRectBuffer->Get_VB();
		}

		m_pGraphicDev->SetStreamSource(0, pVertexBuffer, 0, sizeof(VTXUI));
		m_pGraphicDev->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | (_uint)m_vecListParticle[i].size()));

		m_pGraphicDev->SetStreamSource(1, m_vecInstanceBuffer[i], 0, sizeof(T_PARTICLE_INSTANCE));
		m_pGraphicDev->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

		Set_Constantable();

		pEffect->SetTexture("g_BaseTexture", m_vecTexture[i]);
		_bool bBlackImage = m_vecListParticle[i].front().bBlackBackGroundImage;
		pEffect->SetBool("g_bBlackAlpha", bBlackImage);
		pEffect->CommitChanges();


		//그리기 코드
		if (m_vecListParticle[i].front().bUseCross)
		{
			m_pGraphicDev->SetIndices(m_pCrossRectBuffer->Get_IB());
			m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_pCrossRectBuffer->Get_VtxCnt(), 0, m_pCrossRectBuffer->Get_TriCnt()); // 인덱스 버퍼도 활용하여 출력하는 함수
		}
		else
		{
			m_pGraphicDev->SetIndices(m_pRectBuffer->Get_IB());
			m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_pRectBuffer->Get_VtxCnt(), 0, m_pRectBuffer->Get_TriCnt()); // 인덱스 버퍼도 활용하여 출력하는 함수
		}
	

		m_pGraphicDev->SetStreamSourceFreq(0, 1);
		m_pGraphicDev->SetStreamSourceFreq(1, 1);

		//=================
		//m_pRectBuffer->Render_Buffer();

		m_pShaderCom->EndPass();
		
		
	}
	m_pShaderCom->EndShader();

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

// 깡통
void CParticleObject::Render_Shadow()
{
}

//파티클 오브젝트에 사용한 메모리를 해제하는 함수
void CParticleObject::Free()
{
	Safe_Release(m_pDecl);
	for (auto& pInstanceBuffer : m_vecInstanceBuffer)
	{
		Safe_Release(pInstanceBuffer);
	}
	for (auto& list : m_vecListParticle)
	{
		list.clear();
	}
	CGameObject::Free();
}

//파티클 툴에서 파티클을 저장할때 사용하는 함수
void CParticleObject::Save(HANDLE hFile, DWORD & dwByte)
{


	_int iSize = (_int)m_vecParticleSample.size();
	CUtility::Data_WriteInt(hFile, iSize, dwByte);

	for (auto& tParticlaSample : m_vecParticleSample)
	{
		WriteFile(hFile, &tParticlaSample, sizeof(T_PARTICLE), &dwByte, nullptr);
	}

	iSize = (_int)m_listWaitSpawner.size();
	CUtility::Data_WriteInt(hFile, iSize, dwByte);
	for (auto& tSpawner : m_listWaitSpawner)
	{
		WriteFile(hFile, &tSpawner, sizeof(T_PARTICLE_SPAWNER), &dwByte, nullptr);
	}

	iSize = (_int)m_vecTexturePath.size();
	
	CUtility::Data_WriteInt(hFile, iSize, dwByte);
	WCHAR wszPath[128];
	for (auto& wstrPath : m_vecTexturePath)
	{
		lstrcpy(wszPath, wstrPath.c_str());
		WriteFile(hFile, wszPath, sizeof(WCHAR) * 128, &dwByte, nullptr);
	}


	CUtility::Data_WriteFloat(hFile, m_fLifeTime, dwByte);
	CUtility::Data_WriteFloat(hFile, Get_Scale().x,dwByte);
	CUtility::Data_WriteVec3(hFile, Get_Angle(), dwByte);

	for (_int i = 0; i < 5; i++)
	{
		CUtility::Data_WriteInt(hFile, iTemp[i], dwByte);
	}

	for (_int i = 0; i < 5; i++)
	{
		CUtility::Data_WriteBoolean(hFile, bTemp[i], dwByte);
	}
}

//파티클 툴에서 파티클을 불러올때 사용하는 함수
void CParticleObject::Load(HANDLE hFile, DWORD & dwByte)
{

	m_vecParticleSample.clear();
	m_vecParticleSample.shrink_to_fit();
	m_listWaitSpawner.clear();
	m_vecTexturePath.clear();
	m_vecTexturePath.shrink_to_fit();
	
	_int iSize;

	T_PARTICLE tParticle;
	ZeroMemory(&tParticle, sizeof(T_PARTICLE));
	CUtility::Data_ReadInt(hFile,iSize,dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		ReadFile(hFile, &tParticle, sizeof(T_PARTICLE), &dwByte,nullptr);
		m_vecParticleSample.emplace_back(tParticle);
	}

	T_PARTICLE_SPAWNER tSpawner;
	ZeroMemory(&tSpawner, sizeof(T_PARTICLE_SPAWNER));
	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		ReadFile(hFile, &tSpawner, sizeof(T_PARTICLE_SPAWNER), &dwByte, nullptr);
		m_listWaitSpawner.emplace_back(tSpawner);
	}

	WCHAR wszPath[128];
	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		ReadFile(hFile, wszPath, sizeof(WCHAR) * 128, &dwByte, nullptr);
		m_vecTexturePath.emplace_back(wszPath);


	
		LPDIRECT3DTEXTURE9 pTexture = nullptr;
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wszPath, &pTexture);
		m_vecTexture.emplace_back(pTexture);
	
		m_vecListParticle.emplace_back(list<T_PARTICLE>());
		m_vecInstanceBuffer.emplace_back(LPDIRECT3DVERTEXBUFFER9());
	}
	CUtility::Data_ReadFloat(hFile, m_fLifeTime, dwByte);

	_float fScale;
	CUtility::Data_ReadFloat(hFile, fScale, dwByte);
	Set_Scale(fScale,fScale,fScale);

	_vec3 vRotation;
	CUtility::Data_ReadVec3(hFile, vRotation, dwByte);
	Set_Rotation(vRotation);

	for (_int i = 0; i < 5; i++)
	{
		CUtility::Data_ReadInt(hFile, iTemp[i], dwByte);
	}

	for (_int i = 0; i < 5; i++)
	{
		CUtility::Data_ReadBoolean(hFile, bTemp[i], dwByte);
	}
}

//쉐이더에 필요한 정보를 세팅하는 함수
void CParticleObject::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	//0204mat
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	
}

//깡통
void CParticleObject::Debug_This_In_Console()
{
}

void CParticleObject::Set_ParticleDead()
{
	m_listActiveSpawner.clear();
	m_listWaitSpawner.clear();
}

//프리팹 매니저에 보관하고있는 데이터를 세이브파일의 이름을 통해 불러오는 함수
void CParticleObject::PrefabLoad(wstring wstrPrefabName)
{
	if (wstrPrefabName.size() < 4)
		return;

	m_vecParticleSample.clear();
	m_vecParticleSample.shrink_to_fit();
	m_listWaitSpawner.clear();
	m_vecTexturePath.clear();
	m_vecTexturePath.shrink_to_fit();

	_int iSize=0;
	DWORD dwByte;

	T_PARTICLE tParticle;
	ZeroMemory(&tParticle, sizeof(T_PARTICLE));
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &tParticle, sizeof(T_PARTICLE), &dwByte);
		m_vecParticleSample.emplace_back(tParticle);
	}

	T_PARTICLE_SPAWNER tSpawner;
	ZeroMemory(&tSpawner, sizeof(T_PARTICLE_SPAWNER));
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &tSpawner, sizeof(T_PARTICLE_SPAWNER), &dwByte);
		m_listWaitSpawner.emplace_back(tSpawner);
	}

	WCHAR wszPath[128];
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &iSize, sizeof(_int), &dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, wszPath, sizeof(WCHAR) * 128, &dwByte);
		m_vecTexturePath.emplace_back(wszPath);



		LPDIRECT3DTEXTURE9 pTexture = nullptr;
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wszPath, &pTexture);
		m_vecTexture.emplace_back(pTexture);

		m_vecListParticle.emplace_back(list<T_PARTICLE>());
		m_vecInstanceBuffer.emplace_back(LPDIRECT3DVERTEXBUFFER9());
	}

	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &m_fLifeTime, sizeof(_float), &dwByte);

	_float fScale;
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &fScale, sizeof(_float), &dwByte);
	Set_Scale(fScale, fScale, fScale);
	m_fStartScale = fScale;

	_vec3 vRotation;
	CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &vRotation, sizeof(_vec3), &dwByte);
	Set_Rotation(vRotation);

	_int iTemp;
	for (_int i = 0; i < 5; i++)
	{
		CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &iTemp, sizeof(_int), &dwByte);
	}

	_bool bTemp;
	for (_int i = 0; i < 5; i++)
	{
		CPrefabManager::GetInstance()->ReadPrefab(wstrPrefabName, &bTemp, sizeof(_bool), &dwByte);
	}

	CPrefabManager::GetInstance()->FileReset();
}

//파티클 샘플을 추가하는 함수
void CParticleObject::Add_ParticleSample(T_PARTICLE tParticleSample)
{
	m_vecParticleSample.emplace_back(tParticleSample);
}

//대기중인 스포너 리스트에 스포너를 추가하는 함수
void CParticleObject::Add_WaitSpawner(T_PARTICLE_SPAWNER tSpawner)
{
	m_listWaitSpawner.emplace_back(tSpawner);
}

//텍스쳐를 추가하는 함수
void CParticleObject::Add_Texture(wstring wstrTexturePath)
{
	
	LPDIRECT3DTEXTURE9 pTexture;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev,wstrTexturePath,&pTexture);

	m_vecTexture.emplace_back(pTexture);
	m_vecTexturePath.emplace_back(wstrTexturePath);
}

//추가했던 텍스쳐를 삭제하는 함수
void CParticleObject::Delete_Texture(_int iIndex)
{
	if (m_vecTexture.size() <= iIndex)
	{
		return;
	}

	auto iterTexture = m_vecTexture.begin();
	auto iterPath = m_vecTexturePath.begin();

	for (_int i = 0; i < iIndex; ++i)
	{
		++iterTexture;
		++iterPath;
	}

	*iterTexture = nullptr;
	m_vecTexture.erase(iterTexture);
	m_vecTexturePath.erase(iterPath);
}

//추가했던 파티클 샘플을 삭제하는 함수
void CParticleObject::Delete_ParticleSample(_int iIndex)
{
	if (iIndex >= m_vecParticleSample.size())
		return;

	auto iter = m_vecParticleSample.begin();
	for (_int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	m_vecParticleSample.erase(iter);
}

//대기중인 스포너를 삭제하는 함수
void CParticleObject::Delete_WaitSpawner(_int iIndex)
{
	if (iIndex >= m_listWaitSpawner.size())
		return;

	auto iter = m_listWaitSpawner.begin();
	for (_int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	m_listWaitSpawner.erase(iter);
}

//파티클 샘플 컨테이너 사이즈 반환
_int CParticleObject::Get_ListParticleSampleSize()
{
	return _int(m_vecParticleSample.size());
}

//대기중인 스포너 컨테이너 사이즈 반환
_int CParticleObject::Get_ListWaitSpawnerSize()
{
	return _int(m_listWaitSpawner.size());
}

//현재 활성화 중인 스포너 컨테이너 사이즈 반환
_int CParticleObject::Get_ListActiveSpawnerSize()
{
	return _int(m_listActiveSpawner.size());
}

//현재 보관중인 텍스쳐 컨테이너 사이즈 반환
_int CParticleObject::Get_vecTextureSize()
{
	return _int(m_vecTexturePath.size());
}

//파티클 샘플의 주소를 반환하는 함수
T_PARTICLE* CParticleObject::Get_ParticleSamplePtr(_int iIndex)
{
	if ((_int)m_vecParticleSample.size() <= iIndex)
	{
		return nullptr;
	}

	return &m_vecParticleSample[iIndex];
}

//대기중인 스포너의 주소를 반환하는 함수
T_PARTICLE_SPAWNER* CParticleObject::Get_WaitSpawnerPtr(_int iIndex)
{
	if ((_int)m_listWaitSpawner.size() <= iIndex)
	{
		return nullptr;
	}

	auto iter = m_listWaitSpawner.begin();

	for (_int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	return &(*iter);
}

//현재 활성화중인 스포너의 주소를 반환하는 함수
T_PARTICLE_SPAWNER * CParticleObject::Get_ActiveSpawnerPtr(_int iIndex)
{
	if ((_int)m_listActiveSpawner.size() <= iIndex)
	{
		return nullptr;
	}

	auto iter = m_listActiveSpawner.begin();

	for (_int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	return &(*iter);
}

//텍스쳐의 경로를 반환하는 함수
wstring CParticleObject::Get_TexturePath(_int iIndex)
{
	if ((_int)m_vecTexturePath.size() <= iIndex)
	{
		return L"";
		assert(FALSE);
	}

	return m_vecTexturePath[iIndex];

}


//파티클을 관리하는 파티클 오브젝트의 수명을 설정하는 함수
void CParticleObject::Set_LifeTime(_float fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

//파티클을 관리하는 파티클 오브젝트의 수명을 반환하는 함수
_float CParticleObject::Get_LifeTime()
{
	return m_fLifeTime;
}

//파티클에 필요한 컴포넌트를 추가하는 함수
void CParticleObject::Add_Component()
{
	
	CComponent* pComponent = m_pRectBuffer = static_cast<CRectBuffer*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex"));
	m_mapComponent.emplace(ComTag_RectBuffer, pComponent);

	pComponent = m_pCrossRectBuffer = static_cast<CCrossRectBuffer*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_CrossRectTex"));
	m_mapComponent.emplace(L"CrossRectBuffer", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect"));
	m_mapComponent.emplace(ComTag_Shader, pComponent);


}

//대기중인 스포너의 진입시점을 체크해 활성화 시켜주는 함수
void CParticleObject::WaitSpawnerCheck()
{
	if (m_listWaitSpawner.empty())
	{
		return;
	}

	while (true)
	{
		if (m_listWaitSpawner.empty())
		{
			break;
		}

		if (m_fStartTime + m_listWaitSpawner.front().fEntranceTime < CScene::Get_PlayTime())
		{
			T_PARTICLE_SPAWNER tSpawner= m_listWaitSpawner.front();
			tSpawner.fStartTime = CScene::Get_PlayTime();

			m_listWaitSpawner.pop_front();
			m_listActiveSpawner.emplace_back(tSpawner);
		}
		else
		{
			break;
		}
	}
}

//스포너를 업데이트 해주는 함수, 스포너의 이동과 파티클 소환 그리고 수명에따른 소멸등을 처리한다.
void CParticleObject::SpawnerUpdate(_float fDeltaTime)
{
	if (m_listActiveSpawner.empty())
		return;

	auto& iter = m_listActiveSpawner.begin();

	for (; iter != m_listActiveSpawner.end();)
	{
		if (iter->fStartTime + iter->fLifeTime < CScene::Get_PlayTime())
		{
			iter = m_listActiveSpawner.erase(iter);
			continue;
		}

		switch (iter->eCoordSystem)
		{
		case E_COORD_SYSTEM::SYSTEM_ABSOLUTE:
			AbsoluteSpawnerUpdate(&(*iter),fDeltaTime);
			break;
		case E_COORD_SYSTEM::SYSTEM_LOCAL_ROTATION:
			LocalSpawnerUpdate(&(*iter), fDeltaTime);
			break;
		case E_COORD_SYSTEM::SYSTEM_LOCAL_NONE_ROTATION:
			LocalNoneSpawnerUpdate(&(*iter), fDeltaTime);
			break;
		default:
			assert(FALSE);
			break;
		}

		T_PARTICLE_SPAWNER* pSpawner = &(*iter);
		
		//공통 부분
		if (pSpawner->fSpawnTime + pSpawner->fSpawnInterval < CScene:: Get_PlayTime())
		{
			//파티클 생성
			if (pSpawner->iSpawnSampleIndex < 0 || pSpawner->iSpawnSampleIndex >= (_int)m_vecParticleSample.size())
			{
				//인덱스 초과
				return;
			}

			if (!pSpawner->bMultiSpawn)
			{
				_vec3 vSpawnerLook = VEC_LOOK;
				_vec3 vSpawnerRight = VEC_RIGHT;
				_vec3 vSpawnerUp = VEC_UP;

				_vec3 vSpawnRot = pSpawner->vRot + GetRandomVector(pSpawner->vRandomMinSpawnRot, pSpawner->vRandomMaxSpawnRot);

				if (pSpawner->eCoordSystem != E_COORD_SYSTEM::SYSTEM_ABSOLUTE)
				{
					_matrix matSpawnRot;
					D3DXMatrixRotationYawPitchRoll(&matSpawnRot, vSpawnRot.y*Deg2Rad, vSpawnRot.x*Deg2Rad, vSpawnRot.z*Deg2Rad);

					D3DXVec3TransformNormal(&vSpawnerRight, &vSpawnerRight, &matSpawnRot);
					D3DXVec3Normalize(&vSpawnerRight, &vSpawnerRight);
					D3DXVec3TransformNormal(&vSpawnerUp, &vSpawnerUp, &matSpawnRot);
					D3DXVec3Normalize(&vSpawnerUp, &vSpawnerUp);
					D3DXVec3TransformNormal(&vSpawnerLook, &vSpawnerLook, &matSpawnRot);
					D3DXVec3Normalize(&vSpawnerLook, &vSpawnerLook);
				}

				ParticleSpawn(pSpawner->vPos, vSpawnRot,pSpawner->vRandomMinSpawnPos,pSpawner->vRandomMaxSpawnPos,pSpawner->iSpawnSampleIndex,
					vSpawnerLook, vSpawnerRight, vSpawnerUp);
			}
			else
			{
				_matrix matRot, matTrans, matWorld;
				D3DXMatrixTranslation(&matTrans, pSpawner->vPos.x, pSpawner->vPos.y, pSpawner->vPos.z);
				D3DXMatrixRotationYawPitchRoll(&matRot, pSpawner->vRot.y*Deg2Rad, pSpawner->vRot.x*Deg2Rad, pSpawner->vRot.z*Deg2Rad);

				matWorld = matRot*matTrans;

				for (_int i = 0; i < pSpawner->iMultiSpawnCount; ++i)
				{
					_vec3 vSpawnPos = pSpawner->vStartPos + (pSpawner->vAddPos)*_float(i);
					_vec3 vSpawnRot = pSpawner->vStartRot + (pSpawner->vAddRot)*_float(i);
					
					_vec3 vSpawnLook = VEC_LOOK;
					_vec3 vSpawnRight = VEC_RIGHT;
					_vec3 vSpawnUp = VEC_UP;

					vSpawnRot = pSpawner->vRot + vSpawnRot + GetRandomVector(pSpawner->vRandomMinSpawnRot, pSpawner->vRandomMaxSpawnRot);
					
					if (pSpawner->eCoordSystem != E_COORD_SYSTEM::SYSTEM_ABSOLUTE)
					{
						_matrix matSpawnRot;

					
						D3DXMatrixRotationYawPitchRoll(&matSpawnRot, vSpawnRot.y*Deg2Rad, vSpawnRot.x*Deg2Rad, vSpawnRot.z*Deg2Rad);

						D3DXVec3TransformNormal(&vSpawnRight, &vSpawnRight, &matSpawnRot);
						D3DXVec3Normalize(&vSpawnRight, &vSpawnRight);
						D3DXVec3TransformNormal(&vSpawnUp, &vSpawnUp, &matSpawnRot);
						D3DXVec3Normalize(&vSpawnUp, &vSpawnUp);
						D3DXVec3TransformNormal(&vSpawnLook, &vSpawnLook, &matSpawnRot);
						D3DXVec3Normalize(&vSpawnLook, &vSpawnLook);
					}

					D3DXVec3TransformCoord(&vSpawnPos, &vSpawnPos, &matWorld);
					
					ParticleSpawn(vSpawnPos, pSpawner->vRot+vSpawnRot, pSpawner->vRandomMinSpawnPos, pSpawner->vRandomMaxSpawnPos, pSpawner->iSpawnSampleIndex, vSpawnLook, vSpawnRight, vSpawnUp);
				}
			}
			pSpawner->fSpawnTime = CScene:: Get_PlayTime();
		}

		++iter;
	}

}

//소환된 파티클을 업데이트 해주는 함수, 파티클의 이동과 수명에 따른 소멸을 처리한다.
void CParticleObject::ParticleUpdate(_float fDeltaTime)
{
	for (_int iTextureIndex = 0; iTextureIndex < (_int)m_vecTexture.size(); ++iTextureIndex)
	{
		if (m_vecListParticle[iTextureIndex].empty())
		{
			continue;
		}

		auto& iter = m_vecListParticle[iTextureIndex].begin();
		for (; iter != m_vecListParticle[iTextureIndex].end();)
		{
			if (iter->fStartTime + iter->fLifeTime < CScene::Get_PlayTime())
			{
				iter = m_vecListParticle[iTextureIndex].erase(iter);
				continue;
			}

			switch (iter->eCoordSystem)
			{
			case E_COORD_SYSTEM::SYSTEM_ABSOLUTE:
				AbsoluteParticleUpdate(&(*iter), fDeltaTime);
				break;
			case E_COORD_SYSTEM::SYSTEM_LOCAL_ROTATION:
				LocalParticleUpdate(&(*iter), fDeltaTime);
				break;
			case E_COORD_SYSTEM::SYSTEM_LOCAL_NONE_ROTATION:
				LocalNoneParticleUpdate(&(*iter), fDeltaTime);
				break;
			case E_COORD_SYSTEM::SYSTEM_CENTER:
				CenterParticleUpdate(&(*iter), fDeltaTime);
				break;
			default:
				//좌표계가 설정되어 있지 않음
				assert(FALSE);
				break;
			}

			ParticleSpriteUpdate(&(*iter), fDeltaTime);
			++iter;

		}

	}
}

//현재 활동중인 파티클의 정보에 따라 인스턴싱에 필요한 인스턴싱 버퍼를 만들어 주는 함수
void CParticleObject::SetInstanceBuffer()
{
	// Z 소팅 m_vecListParticle[iTextureIndex]

	for (_int iTextureIndex=0;iTextureIndex<(_int)m_vecTexture.size();++iTextureIndex)
	{
		if (m_vecListParticle[iTextureIndex].empty())
		{
			continue;
		}
		//m_vecListParticle[iTextureIndex].sort();

		Safe_Release(m_vecInstanceBuffer[iTextureIndex]);
		m_pGraphicDev->CreateVertexBuffer((_uint)(m_vecListParticle[iTextureIndex].size() * sizeof(T_PARTICLE_INSTANCE)),
			0, 0, D3DPOOL_MANAGED, &m_vecInstanceBuffer[iTextureIndex], 0);

		T_PARTICLE_INSTANCE* pInstanceVertex = nullptr;
		m_vecInstanceBuffer[iTextureIndex]->Lock(0, 0, (void**)&pInstanceVertex, 0);

		auto& iter = m_vecListParticle[iTextureIndex].begin();
		for (_uint i = 0; i < (_int)m_vecListParticle[iTextureIndex].size(); ++i, ++iter)
		{
			_matrix matScale, matTrans, matRot;

			D3DXMatrixScaling(&matScale, iter->vRenderScale.x, iter->vRenderScale.y, iter->vRenderScale.z);
			D3DXMatrixTranslation(&matTrans, iter->vPos.x, iter->vPos.y, iter->vPos.z);

			if (!iter->bIndependent)
			{
				iter->matParentWorld = m_pTransformCom->Get_WorldMatrix();
			}

			if (!iter->bBillBoard)
			{	
				_matrix matOffSetRot;
				D3DXMatrixRotationYawPitchRoll(&matOffSetRot, (iter->vOffSetRot.y) * Deg2Rad, (iter->vOffSetRot.x) * Deg2Rad, (iter->vOffSetRot.z) * Deg2Rad);
				D3DXMatrixRotationYawPitchRoll(&matRot, (iter->vRot.y) * Deg2Rad, (iter->vRot.x) * Deg2Rad, (iter->vRot.z) * Deg2Rad);
				matRot = matOffSetRot*matRot;
			}
			else
			{
				D3DXMatrixRotationYawPitchRoll(&matRot, (iter->vRot.y) * Deg2Rad, (iter->vRot.x) * Deg2Rad, (iter->vRot.z) * Deg2Rad);
			}
			pInstanceVertex[i].vColor = iter->vRenderColor;
			pInstanceVertex[i].matWorld = matScale*matRot*matTrans*(iter->matParentWorld);

			//스프라이트 UV 설정
			if (iter->iSpriteX < 2 && iter->iSpriteY < 2)
			{
				pInstanceVertex[i].vUV = _vec4(0.f, 0.f, 1.f, 1.f);
			}
			else
			{
				_int iIndexX = iter->iSpriteIndex % iter->iSpriteX;
				_int iIndexY = iter->iSpriteIndex / iter->iSpriteX;

				_float fStartX = iter->fSpriteWidth * iIndexX;
				_float fStartY = iter->fSpriteHeight * iIndexY;
				pInstanceVertex[i].vUV = _vec4(fStartX, fStartY, iter->fSpriteWidth, iter->fSpriteHeight);
			}

			if (iter->bBillBoard)
			{
				_matrix matWorld = pInstanceVertex[i].matWorld;
				_vec3 vPos = _vec3(matWorld._41, matWorld._42, matWorld._43);
				_matrix matInvView, matView;
				m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
				D3DXMatrixInverse(&matInvView, 0, &matView);
				_vec3 vLook ,vUp;
				D3DXVec3Normalize(&vLook, &(_vec3(matInvView._41, matInvView._42, matInvView._43) - vPos));
				D3DXVec3Normalize(&vUp, &(_vec3(matInvView._21, matInvView._22, matInvView._23)));
				D3DXMatrixLookAtLH(&pInstanceVertex[i].matWorld, &vPos,&(vPos - vLook), &vUp);
				D3DXMatrixInverse(&pInstanceVertex[i].matWorld, 0, &pInstanceVertex[i].matWorld);
				_matrix matParentScale;
				D3DXMatrixScaling(&matParentScale, Get_Scale().x, Get_Scale().y, Get_Scale().z);
			
				_float fDegree = 0.f;

				if (iter->bBillBoardLookDir)
				{
					_vec3 vParticleLook = _vec3(matWorld._31, matWorld._32, matWorld._33);
					D3DXVec3TransformNormal(&vParticleLook, &vParticleLook, &matView);

					vParticleLook.z = 0.f;

					fDegree = CUtility::GetAngleToTarget_vec3(vParticleLook, _vec3(0.f, 1.f, 0.f));
					if (vParticleLook.x < 0)
					{
						fDegree = -fDegree;
					}
				}
				_matrix matRotZ;
				D3DXMatrixRotationZ(&matRotZ,D3DXToRadian((iter->fBillboardZRot - fDegree)));

				pInstanceVertex[i].matWorld = matScale * matParentScale * matRotZ * pInstanceVertex[i].matWorld;


			}
			
		}
		m_vecInstanceBuffer[iTextureIndex]->Unlock();
	}

}

//Absolute타입의 스포너의 이동을 처리해주는 함수, 자기자신의 로테이션 값과 상관없이 이동한다.
void CParticleObject::AbsoluteSpawnerUpdate(T_PARTICLE_SPAWNER * pSpawner, _float fDeltaTime)
{
	pSpawner->vPos += GetRandomVector(pSpawner->vRandomMinAddPos, pSpawner->vRandomMaxAddPos)*fDeltaTime;
	pSpawner->vRot += GetRandomVector(pSpawner->vRandomMinAddRot, pSpawner->vRandomMaxAddRot)*fDeltaTime;
}

//Local 타입의 스포너의 이동을 처리해주는 함수, 자기자신의 로테이션 값을 통해 이동한다.
void CParticleObject::LocalSpawnerUpdate(T_PARTICLE_SPAWNER * pSpawner, _float fDeltaTime)
{
	_matrix matRot;
	pSpawner->vRot += GetRandomVector(pSpawner->vRandomMinAddRot, pSpawner->vRandomMaxAddRot)*fDeltaTime;
	D3DXMatrixRotationYawPitchRoll(&matRot, pSpawner->vRot.y*Deg2Rad, pSpawner->vRot.x*Deg2Rad, pSpawner->vRot.z*Deg2Rad);

	_vec3 vRight = VEC_RIGHT;
	_vec3 vUp = VEC_UP;
	_vec3 vLook = VEC_LOOK;

	D3DXVec3TransformNormal(&vRight,&vRight,&matRot);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
	D3DXVec3Normalize(&vLook, &vLook);


	_vec3 vAddPos=GetRandomVector(pSpawner->vRandomMinAddPos, pSpawner->vRandomMaxAddPos)*fDeltaTime;
	pSpawner->vPos += vRight*vAddPos.x + vUp*vAddPos.y + vLook*vAddPos.z;


}

//깡통
void CParticleObject::LocalNoneSpawnerUpdate(T_PARTICLE_SPAWNER * pSpawner, _float fDeltaTime)
{

}

//Absolute타입의 파티클의 이동을 처리하는 함수, 스포너의 로테이션값 영향을 받지않고 자신의 로테이션값 또한 받지않는다.
void CParticleObject::AbsoluteParticleUpdate(T_PARTICLE * pParticle, _float fDeltaTime)
{
	pParticle->vPos += GetRandomVector(pParticle->vRandomMinAddPos, pParticle->vRandomMaxAddPos)*fDeltaTime;
	pParticle->vRot += GetRandomVector(pParticle->vRandomMinAddRot, pParticle->vRandomMaxAddRot)*fDeltaTime;
	
	ParticleScaleUpdate(pParticle);
	ParticleGravityUpdate(pParticle, fDeltaTime);

	ParticleColorUpdate(pParticle);
		
}

//Local 타입의 파티클의 이동을 처리하는 함수, 스포너의 로테이션값과 자신의 로테이션값의 영향을 받는다.
void CParticleObject::LocalParticleUpdate(T_PARTICLE * pParticle, _float fDeltaTime)
{
	_matrix matRot;
	pParticle->vRot += GetRandomVector(pParticle->vRandomMinAddRot, pParticle->vRandomMaxAddRot)*fDeltaTime;
	D3DXMatrixRotationYawPitchRoll(&matRot, pParticle->vRot.y*Deg2Rad, pParticle->vRot.x*Deg2Rad, pParticle->vRot.z*Deg2Rad);
	
	ParticleScaleUpdate(pParticle);
	ParticleColorUpdate(pParticle);

	_vec3 vRight = VEC_RIGHT;
	_vec3 vUp = VEC_UP;
	_vec3 vLook = VEC_LOOK;

	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
	D3DXVec3Normalize(&vLook, &vLook);


	_vec3 vAddPos = GetRandomVector(pParticle->vRandomMinAddPos, pParticle->vRandomMaxAddPos)*fDeltaTime;
	pParticle->vPos += vRight*vAddPos.x + vUp*vAddPos.y + vLook*vAddPos.z;

	ParticleGravityUpdate(pParticle, fDeltaTime);
}

//LocalNoneRotation타입의 파티클의 이동을 처리하는 함수, 스포너의 영향을 받지만 자신의 로테이션값의 영향을 받지않는다.
void CParticleObject::LocalNoneParticleUpdate(T_PARTICLE * pParticle, _float fDeltaTime)
{
	pParticle->vRot += GetRandomVector(pParticle->vRandomMinAddRot, pParticle->vRandomMaxAddRot)*fDeltaTime;

	ParticleScaleUpdate(pParticle);
	ParticleColorUpdate(pParticle);

	_vec3 vAddPos = GetRandomVector(pParticle->vRandomMinAddPos, pParticle->vRandomMaxAddPos)*fDeltaTime;
	pParticle->vPos += pParticle->vRight*vAddPos.x + pParticle->vUp*vAddPos.y + pParticle->vLook*vAddPos.z;

	ParticleGravityUpdate(pParticle, fDeltaTime);
}


void CParticleObject::CenterParticleUpdate(T_PARTICLE * pParticle, _float fDeltaTime)
{
	_matrix matRot;
	pParticle->vRot += GetRandomVector(pParticle->vRandomMinAddRot, pParticle->vRandomMaxAddRot)*fDeltaTime;
	D3DXMatrixRotationYawPitchRoll(&matRot, pParticle->vRot.y*Deg2Rad, pParticle->vRot.x*Deg2Rad, pParticle->vRot.z*Deg2Rad);
	
	ParticleScaleUpdate(pParticle);
	ParticleColorUpdate(pParticle);

	//_vec3 vCenterPos = _vec3(pParticle->matParentWorld._41, pParticle->matParentWorld._42, pParticle->matParentWorld._43);
	_vec3 vLook = pParticle->vPos;
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vWorldUp = VEC_UP;
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &vWorldUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	

	_vec3 vAddPos = GetRandomVector(pParticle->vRandomMinAddPos, pParticle->vRandomMaxAddPos)*fDeltaTime;
	pParticle->vPos += vRight*vAddPos.x + vWorldUp*vAddPos.y + vLook*vAddPos.z;

	ParticleGravityUpdate(pParticle, fDeltaTime);
}

void CParticleObject::ParticleSpriteUpdate(T_PARTICLE * pParticle, _float fDeltaTime)
{
	if (!pParticle)
	{
		return;
	}
	if (pParticle->iSpriteX < 2 && pParticle->iSpriteY < 2)
	{
		return;
	}

	if (pParticle->fSpriteTimer + pParticle->fCycle < CScene::Get_PlayTime())
	{
		++pParticle->iSpriteIndex;
		pParticle->iSpriteIndex = pParticle->iSpriteIndex % (pParticle->iSpriteX * pParticle->iSpriteY);
		pParticle->fSpriteTimer = CScene::Get_PlayTime();
	}
}

//파티클에 설정된 중력값을 적용하는 함수
void CParticleObject::ParticleGravityUpdate(T_PARTICLE * pParticle,_float fDeltaTime)
{
	pParticle->fCurrentGravity += pParticle->fGravity*fDeltaTime;
	if (pParticle->fCurrentGravity > pParticle->fMaxGravity)
	{
		pParticle->fCurrentGravity = pParticle->fMaxGravity;
	}
	pParticle->vPos.y -= pParticle->fCurrentGravity * fDeltaTime;
}

void CParticleObject::ParticleColorUpdate(T_PARTICLE * pParticle)
{
	_float fPlayTime = (CScene:: Get_PlayTime() - pParticle->fStartTime) / pParticle->fLifeTime;
	_vec4 vColorDiff = pParticle->vEndColor - pParticle->vStartColor;
	if (fPlayTime > 1.f)
	{
		fPlayTime = 1.f;
	}
	pParticle->vRenderColor = pParticle->vStartColor + vColorDiff*fPlayTime;
}

void CParticleObject::ParticleScaleUpdate(T_PARTICLE * pParticle)
{
	_float fPlayTime = (CScene:: Get_PlayTime() - pParticle->fStartTime) / pParticle->fLifeTime;
	_vec3 vScaleDiff = pParticle->vEndScale - pParticle->vScale;
	if (fPlayTime > 1.f)
	{
		fPlayTime = 1.f;
	}
	pParticle->vRenderScale = pParticle->vScale + vScaleDiff*fPlayTime;
}

//파티클을 빌보드시키려면 필요한 로테이션값을 반환하는 함수
_vec3 CParticleObject::GetBillBoardRotation(_vec3 vPos)
{
	_matrix matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);


	_vec3 vTargetPos=_vec3(matInvView._41, matInvView._42, matInvView._43);


	return CMyMath::GetLookRotation(vPos,vTargetPos);
}

//_vec3 와 _vec3 의 랜덤값을 반환하는 함수
_vec3 CParticleObject::GetRandomVector(_vec3 vMin, _vec3 vMax)
{
	_vec3 vResult;
	vResult.x = CWall512::GetInstance()->GetFloatValue(vMin.x, vMax.x);
	vResult.y = CWall512::GetInstance()->GetFloatValue(vMin.y, vMax.y);
	vResult.z = CWall512::GetInstance()->GetFloatValue(vMin.z, vMax.z);

	return vResult;
}

//_vec4 와 _vec4 의 랜덤값을 반환하는 함수
_vec4 CParticleObject::GetRandomVector4(_vec4 vMin, _vec4 vMax)
{
	_vec4 vResult;
	vResult.x = CWall512::GetInstance()->GetFloatValue(vMin.x, vMax.x);
	vResult.y = CWall512::GetInstance()->GetFloatValue(vMin.y, vMax.y);
	vResult.z = CWall512::GetInstance()->GetFloatValue(vMin.z, vMax.z);
	vResult.w = CWall512::GetInstance()->GetFloatValue(vMin.w, vMax.w);
	return vResult;
}

//스포너의 정보에 따라 파티클을 스폰해주는 함수
void CParticleObject::ParticleSpawn(_vec3 vPos, _vec3 vRot, _vec3 vMinSpawnPos, _vec3 vMaxSpawnPos, _int iSampleIndex, _vec3 vSpawnerLook, _vec3 vSpawnerRight, _vec3 vSpawnerUp)
{
	T_PARTICLE tParticle = m_vecParticleSample[iSampleIndex];

	tParticle.fLifeTime = CWall512::GetInstance()->GetFloatValue(tParticle.fLifeTime, tParticle.fMaxLifeTime);
	tParticle.vStartColor = GetRandomVector4(tParticle.vStartColor, tParticle.vStartColorMax);
	tParticle.vScale = GetRandomVector(tParticle.vScale, tParticle.vScaleMax);
	_vec3 vSpawnPos = GetRandomVector(vMinSpawnPos, vMaxSpawnPos);
	tParticle.vPos = vPos + vSpawnerLook * vSpawnPos.z + vSpawnerRight * vSpawnPos.x + vSpawnerUp * vSpawnPos.y;
	tParticle.vRot = vRot;
	tParticle.vRenderScale = m_vecParticleSample[iSampleIndex].vScale;

	tParticle.fStartTime = CScene::Get_PlayTime();

	tParticle.fCycle = tParticle.fLifeTime / (tParticle.fCycle * _float(tParticle.iSpriteX * tParticle.iSpriteY));
	tParticle.fSpriteTimer = CScene::Get_PlayTime();

	tParticle.fSpriteWidth = 1.f / tParticle.iSpriteX;
	tParticle.fSpriteHeight = 1.f / tParticle.iSpriteY;
	tParticle.iSpriteIndex = 0;

	if (tParticle.iTextureIndex < 0 || tParticle.iTextureIndex >= (_int)m_vecTexture.size())
	{
		return;
	}

	if (tParticle.eCoordSystem == E_COORD_SYSTEM::SYSTEM_LOCAL_NONE_ROTATION)
	{
		tParticle.vLook = vSpawnerLook;
		tParticle.vRight = vSpawnerRight;
		tParticle.vUp = vSpawnerUp;
	}

	tParticle.matParentWorld = m_pTransformCom->Get_WorldMatrix();

	m_vecListParticle[tParticle.iTextureIndex].emplace_back(tParticle);
}


//파티클 오브젝트 생성, 이 경우 이미 저장된 파티클을 불러오는 생성자
CParticleObject * CParticleObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrParticleTag)
{
	CParticleObject* pInstance = new CParticleObject(pGraphicDev,eObjTag);

	if (FAILED(pInstance->Ready_Object(wstrParticleTag)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

//파티클 오브젝트 생성, 이 경우 아무 기능도 없는 깡통 상태의 파티클 생성
CParticleObject * CParticleObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CParticleObject* pInstance = new CParticleObject(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}


#include "SMesh.h"
#include "ProtoMgr.h"
#include "Renderer.h"

CSMesh::CSMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pOriMesh(nullptr)
	, m_pMesh(nullptr)
	, m_pAdjacency(nullptr)
	, m_pSubset(nullptr)
	, m_pMtrl(nullptr)
	, m_dwSubsetCnt(0)
	, m_ppTexture(nullptr)
	, m_dwStride(0)
	, m_pVtxPos(nullptr)
	, m_dwVtxCnt(0)
{

}

CSMesh::CSMesh(const CSMesh & rhs)
	: CComponent(rhs)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_pMtrl(rhs.m_pMtrl)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwStride(rhs.m_dwStride)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_pAttributeTable(rhs.m_pAttributeTable)
	, m_dwNumAttributeGroups(rhs.m_dwNumAttributeGroups)
	, m_vector_GlowOption(rhs.m_vector_GlowOption)
{
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppNormalTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppSpecularTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppMaskTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	
	ZeroMemory(m_ppTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	ZeroMemory(m_ppNormalTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	ZeroMemory(m_ppSpecularTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	ZeroMemory(m_ppMaskTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	
	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];

		if (rhs.m_ppNormalTexture[i])
		{
			m_ppNormalTexture[i] = rhs.m_ppNormalTexture[i];
		}
		else
		{
			Safe_Release(m_ppNormalTexture[i]);
		}

		if (rhs.m_ppSpecularTexture[i])
		{
			m_ppSpecularTexture[i] = rhs.m_ppSpecularTexture[i];
		}
		else
		{
			Safe_Release(m_ppSpecularTexture[i]);
		}

		if (rhs.m_ppMaskTexture[i])
		{
			m_ppMaskTexture[i] = rhs.m_ppMaskTexture[i];
		}
		else
		{
			Safe_Release(m_ppMaskTexture[i]);
		}		
	}


	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();
}

CSMesh::~CSMesh()
{
}

HRESULT CSMesh::Ready_Meshes(const wstring& pFilePath, const wstring& pFileName)
{	
	_tchar	szFullPath[MAX_PATH] = L"";

	// 메쉬의 최종 경로를 만들어주는 코드
	// Bin\Resource\Mesh\StaticMesh\Sword
	lstrcpy(szFullPath, pFilePath.c_str());
	lstrcat(szFullPath, pFileName.c_str());

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev,
								&m_pAdjacency,
								&m_pSubset,
								NULL,
								&m_dwSubsetCnt,
								&m_pOriMesh)))
								return E_FAIL;


	//// 메쉬 정점 정보중 노말 값이 없는 경우
	//if (!(dwFVF & D3DFVF_NORMAL))
	//{
	//	// 현재 메쉬가 지닌 정점의 FVF정보를 2번 째 인자값의 형태로 복제하는 함수
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
	//	// 인접한 정점 또는 폴리곤 정보를 근거로 법선을 만들어주는 함수
	//	D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	//}
	//else
	//{
	//	m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	//}

	if (FAILED(m_pOriMesh->CloneMesh(m_pOriMesh->GetOptions(), vertexStaticDecl, m_pGraphicDev, &m_pMesh)))
	{
		return E_FAIL;
	}
	
	m_pMesh->GetAttributeTable(NULL, &m_dwNumAttributeGroups);
	m_pAttributeTable = new D3DXATTRIBUTERANGE[m_dwNumAttributeGroups];
	m_pMesh->GetAttributeTable(m_pAttributeTable, NULL);

	//D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	//D3DXComputeTangent(m_pMesh, 0, 0, 0, 0, (_ulong*)m_pAdjacency->GetBufferPointer());

	m_pGraphicDev->CreateVertexDeclaration(vertexDecl, &m_pVertexDeclaration);

	_ulong  dwFVF = m_pOriMesh->GetFVF();

	// m_pSubset가 갖고있는 정보의 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_dwVtxCnt = m_pMesh->GetNumVertices();  // 메쉬가 지닌 정점의 개수를 반환
	m_pVtxPos = new _vec3[m_dwVtxCnt];

	void *		pVertex = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte			byOffSet = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = (_ubyte)Decl[i].Offset;
			break;
		}
	}
	// FVF 정보를 토대로 정점의 크기를 계산해주는 함수
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + byOffSet)));
	}

	// m_Subset가 갖고있는 정보의 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();


	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	ZeroMemory(m_ppTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	m_ppNormalTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	ZeroMemory(m_ppNormalTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	m_ppSpecularTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	ZeroMemory(m_ppSpecularTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	m_ppMaskTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	ZeroMemory(m_ppMaskTexture, sizeof(LPDIRECT3DTEXTURE9) * m_dwSubsetCnt);
	
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";
		_tchar	szNormalName[256] = L"";
		_tchar	szSpecularName[256] = L"";

		//Glow
		_tchar	szMaskName[256] = L"";
		T_GlowOption tGlowOption = T_GlowOption(); 

		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, (_int)strlen(m_pMtrl[i].pTextureFilename), szFileName, 256);

		if (!lstrcmp(L"TO14_T_LNA_Hole01_C.tga", szFileName))
		{
			int temp = 0;
			temp = 3;
		}
		// 메쉬 텍스쳐의 최종 경로를 만들어주는 코드
		lstrcpy(szFullPath, pFilePath.c_str());
		lstrcat(szFullPath, szFileName);

		/*if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i])))
		{
			return E_FAIL;
		}*/
		if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &m_ppTexture[i])))
		{
			return E_FAIL;
		}


		// 노말 텍스쳐 읽기
		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, (_int)strlen(m_pMtrl[i].pTextureFilename) - 4, szNormalName, 256);

		lstrcat(szNormalName, L"N.tga");

		if (wcswcs(szFullPath, L"slumpia_01"))
		{
			int ua = 1;
			//pDerivedMeshContainer->bFace = TRUE;
		}

		lstrcpy(szFullPath, pFilePath.c_str());
		lstrcat(szFullPath, szNormalName);
		/*if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppNormalTexture[i])))
		{
			int temp;
			temp = 3;
		}*/
		if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &m_ppNormalTexture[i])))
		{
			int temp;
			temp = 3;
		}


		// Specular 텍스쳐 읽기
		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, (_int)strlen(m_pMtrl[i].pTextureFilename) - 4, szSpecularName, 256);

		lstrcat(szSpecularName, L"_Spec.png");

		lstrcpy(szFullPath, pFilePath.c_str());
		lstrcat(szFullPath, szSpecularName);
		/*if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppSpecularTexture[i])))
		{
			int temp;
			temp = 3;
		}*/
		if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &m_ppSpecularTexture[i])))
		{
			int temp;
			temp = 3;
		}
		
		// Mask 텍스쳐 읽기
		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, (_int)strlen(m_pMtrl[i].pTextureFilename) - 4, szMaskName, 256);

		lstrcat(szMaskName, L"_Mask.png");
		
		lstrcpy(szFullPath, pFilePath.c_str());
		lstrcat(szFullPath, szMaskName);
		if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &m_ppMaskTexture[i])))
		{
			int temp;
			temp = 3;
		}


		m_vector_GlowOption.emplace_back();
	}

	return S_OK;
}

void CSMesh::Render_Meshes(CShader* pShaderCom)
{
	LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();

	// 디버그용 코드입니다~
	_int iSize_1 = (_int)m_vector_GlowOption.size();
	_int iSize_2 = m_dwSubsetCnt;

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetVertexDeclaration(m_pVertexDeclaration);


		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
		if (m_ppNormalTexture[i])
		{
			pEffect->SetTexture("g_ShadeTexture", m_ppNormalTexture[i]);
		}


		if (m_ppSpecularTexture[i])
		{
			pEffect->SetTexture("g_SpecularTexture", m_ppSpecularTexture[i]);
			pEffect->SetBool("g_bSpecularMap", TRUE);
		}
		else
		{
			pEffect->SetBool("g_bSpecularMap", FALSE);
		}


		if (m_ppSpecularTexture[i])
		{
			pEffect->SetTexture("g_SpecularTexture", m_ppSpecularTexture[i]);
			pEffect->SetBool("g_bSpecularMap", TRUE);
		}
		else
		{
			pEffect->SetBool("g_bSpecularMap", FALSE);
		}

		//////////////////////////////////////////////////////////////////////////////
		// Glow		

		if (CRenderer::GetInstance()->Get_BaseGlow())
		{
			pEffect->SetFloat("g_fBaseGlow", m_vector_GlowOption[i].fBaseGlow);
			// 디버그용 -> 전체 Smesh에 글로우가 먹는다
			//pEffect->SetFloat("g_fBaseGlow", 0.5f);
		}

		if (m_ppMaskTexture[i] && CRenderer::GetInstance()->Get_TexGlow())
		{
			pEffect->SetFloat("g_fTexGlowR", m_vector_GlowOption[i].fTexGlowR);
			pEffect->SetFloat("g_fTexGlowG", m_vector_GlowOption[i].fTexGlowG);
			pEffect->SetFloat("g_fTexGlowB", m_vector_GlowOption[i].fTexGlowB);

			pEffect->SetTexture("g_MaskTexture", m_ppMaskTexture[i]);
		}
		else
		{
			pEffect->SetFloat("g_fTexGlowR", 0.f);
			pEffect->SetFloat("g_fTexGlowG", 0.f);
			pEffect->SetFloat("g_fTexGlowB", 0.f);

			CRenderer::GetInstance()->Set_DefualtMaskTexture(pEffect, "g_MaskTexture");
		}

		pEffect->SetFloat("g_fGlow_U_Min", m_vector_GlowOption[i].fGlow_U_Min);
		pEffect->SetFloat("g_fGlow_U_Max", m_vector_GlowOption[i].fGlow_U_Max);
		pEffect->SetFloat("g_fGlow_V_Min", m_vector_GlowOption[i].fGlow_V_Min);
		pEffect->SetFloat("g_fGlow_V_Max", m_vector_GlowOption[i].fGlow_V_Max);

		pEffect->SetFloat("g_fGlowPower", m_vector_GlowOption[i].fGlowPower);

		pEffect->SetFloat("g_fGlowColorR", m_vector_GlowOption[i].vGlowColor.x);
		pEffect->SetFloat("g_fGlowColorG", m_vector_GlowOption[i].vGlowColor.y);
		pEffect->SetFloat("g_fGlowColorB", m_vector_GlowOption[i].vGlowColor.z);
		
		//////////////////////////////////////////////////////////////////////////////

		pEffect->CommitChanges();
		m_pMesh->DrawSubset(i);
	}
}

void CSMesh::Render_Meshes_Instance(CShader * pShaderCom)
{
	LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();

	for (_int i = 0; i < (_int)m_dwNumAttributeGroups; ++i)
	{
		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
		if (m_ppNormalTexture[i])
		{
			pEffect->SetTexture("g_ShadeTexture", m_ppNormalTexture[i]);
		}
		if (m_ppSpecularTexture[i])
		{
			pEffect->SetTexture("g_SpecularTexture", m_ppSpecularTexture[i]);
			pEffect->SetBool("g_bSpecularMap", TRUE);
		}
		else
		{
			pEffect->SetBool("g_bSpecularMap", FALSE);
		}

		pEffect->CommitChanges();
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_pAttributeTable[i].VertexCount, m_pAttributeTable[i].FaceStart * 3, m_pAttributeTable[i].FaceCount);

	}
}

void CSMesh::Set_Glow_Option(_int iContainerIndex, _float bBaseGlow, _vec3 vTexGlowRGB, _float bGlowPower, _float fGlow_U_Min, _float fGlow_U_Max, _float fGlow_V_Min, _float fGlow_V_Max, _vec3 vGlowColor/*=_vec3(1.f, 1.f, 1.f)*/)
{
	T_GlowOption& tOption = m_vector_GlowOption[iContainerIndex];
	
	tOption.fBaseGlow	= bBaseGlow;
	tOption.fTexGlowR	= vTexGlowRGB.x;
	tOption.fTexGlowG	= vTexGlowRGB.y;
	tOption.fTexGlowB	= vTexGlowRGB.z;
	tOption.fGlowPower	= bGlowPower;

	tOption.fGlow_U_Min = fGlow_U_Min;
	tOption.fGlow_U_Max = fGlow_U_Max;
	tOption.fGlow_V_Min = fGlow_V_Min;
	tOption.fGlow_V_Max = fGlow_V_Max;

	tOption.vGlowColor = vGlowColor;	
}

void CSMesh::Set_Glow_Option(_int iContainerIndex, T_GlowOption tGlowOption)
{
	m_vector_GlowOption[iContainerIndex] = tGlowOption;
}

T_GlowOption & CSMesh::Get_Glow_Option(_int iContainerIndex)
{
	return m_vector_GlowOption[iContainerIndex];
}

_bool CSMesh::PickingMesh(HWND hWnd, _vec3 * pOutPos, _float * fOutDis, _matrix _matWorld)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 윈도우 좌표를 투영 좌표로 변환
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 좌표를 뷰스페이스 좌표로 변환
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰스페이스 좌표를 월드 좌표로 변환
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	// 광선을 발사하는 위치 vRayPos, 광선이 향하는 방향을 vRayDir
	_vec3		vRayPos, vRayDir;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 좌표에서 로컬 좌표로 변환
	_matrix	 matWorld = _matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	BOOL bCOL;
	DWORD dwFaceIndex;
	_float fV, fU, fDist;
	LPD3DXBUFFER pBuffer;
	DWORD dwHit;
	D3DXIntersect(m_pMesh, &vRayPos, &vRayDir, &bCOL, &dwFaceIndex, &fU, &fV, &fDist, &pBuffer, &dwHit);

	if (bCOL)
	{
	
		_vec3 vResultPos = vRayPos + vRayDir*fDist;
		D3DXVec3TransformCoord(pOutPos,&vResultPos,&_matWorld);
		*fOutDis = fDist;
		return TRUE;
	}


	
	return FALSE;
}

_bool CSMesh::PickingMesh(_vec3 vWorldRayStart, _vec3 vWorldRayDir, _vec3 * pOutPos, _float * fOutDis, _matrix _matWorld)
{
	
	// 광선을 발사하는 위치 vRayPos, 광선이 향하는 방향을 vRayDir
	_vec3		vRayPos, vRayDir;

	vRayPos = vWorldRayStart;
	vRayDir = vWorldRayDir;


	// 월드 좌표에서 로컬 좌표로 변환
	_matrix	 matWorld = _matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	BOOL bCOL;
	DWORD dwFaceIndex;
	_float fV, fU, fDist;
	LPD3DXBUFFER pBuffer;
	DWORD dwHit;
	D3DXIntersect(m_pMesh, &vRayPos, &vRayDir, &bCOL, &dwFaceIndex, &fU, &fV, &fDist, &pBuffer, &dwHit);

	if (bCOL)
	{

		_vec3 vResultPos = vRayPos + vRayDir * fDist;
		D3DXVec3TransformCoord(pOutPos, &vResultPos, &_matWorld);
		*fOutDis = fDist;
		return TRUE;
	}



	return FALSE;
}


void CSMesh::Save(HANDLE hFile)
{
}

void CSMesh::Load(HANDLE hFile)
{
}

CSMesh * CSMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pFilePath, const wstring& pFileName)
{
	CSMesh*		pInstance = new CSMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CSMesh::Clone()
{
	return new CSMesh(*this);
}

void CSMesh::Free()
{
	/*for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		Safe_Release(m_ppTexture[i]);

		if (m_ppNormalTexture[i])
		{
			Safe_Release(m_ppNormalTexture[i]);
		}
		if (m_ppSpecularTexture[i])
		{
			Safe_Release(m_ppSpecularTexture[i]);
		}
	}*/

	Safe_Delete_Array(m_ppTexture);
	Safe_Delete_Array(m_ppNormalTexture);
	Safe_Delete_Array(m_ppSpecularTexture);
	Safe_Delete_Array(m_ppMaskTexture);

	if (FALSE == m_bClone)
	{
		Safe_Delete_Array(m_pVtxPos);
		Safe_Delete_Array(m_pAttributeTable);
	}

	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pVertexDeclaration);

	
	
	CComponent::Free();
}

#include "HierarchyLoader.h"
#include "ProtoMgr.h"
USING(Engine)

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

CHierarchyLoader::~CHierarchyLoader()
{

}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath)
{
	return new CHierarchyLoader(pGraphicDev, pPath);
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*		pDerivedMatrix = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedMatrix, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pDerivedMatrix->Name, Name);

	pDerivedMatrix->CombinedTransformMatrix = *D3DXMatrixIdentity(&pDerivedMatrix->TransformationMatrix);

	*ppNewFrame = pDerivedMatrix;

	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::CreateMeshContainer(LPCSTR Name,
	const D3DXMESHDATA * pMeshData,
	const D3DXMATERIAL * pMaterials,
	const D3DXEFFECTINSTANCE * pEffectInstances,
	DWORD NumMaterials,
	const DWORD * pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));


	T_GlowOption tGlowOption = T_GlowOption();
	pDerivedMeshContainer->fBaseGlow	= tGlowOption.fBaseGlow;
	pDerivedMeshContainer->fTexGlowR	= tGlowOption.fTexGlowR;
	pDerivedMeshContainer->fTexGlowG	= tGlowOption.fTexGlowG;
	pDerivedMeshContainer->fTexGlowB	= tGlowOption.fTexGlowB;
	pDerivedMeshContainer->fGlowPower	= tGlowOption.fGlowPower;	
	pDerivedMeshContainer->fGlow_U_Min	= tGlowOption.fGlow_U_Min;
	pDerivedMeshContainer->fGlow_U_Max	= tGlowOption.fGlow_U_Max;
	pDerivedMeshContainer->fGlow_V_Min	= tGlowOption.fGlow_V_Min;
	pDerivedMeshContainer->fGlow_V_Max	= tGlowOption.fGlow_V_Max;
	pDerivedMeshContainer->vGlowColor	= tGlowOption.vGlowColor;


	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	// 메쉬가 지닌 폴리곤의 개수를 반환
	_ulong dwNumFaces = pMesh->GetNumFaces();

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);


	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppNormalTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppNormalTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppSpecularTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppSpecularTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppMaskTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppMaskTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);
	
	if (0 != NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			_tchar	szFullPath[MAX_PATH] = L"";
			_tchar	szFileName[256] = L"";
			_tchar	szNormalName[256] = L"";
			_tchar	szSpecularName[256] = L"";
			_tchar	szMaskName[256] = L"";

			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename,
				(_int)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename), szFileName, 256);

			// 메쉬 텍스처의 최종 경로를 만들어주는 코드
			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if (FALSE == lstrcmp(szFileName, L"material_0"))
			{
				continue;
			}

			if (FALSE == lstrcmp(szFileName, L"Eye"))
			{
				continue;
			}

			/*if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppTexture[i])))
				return E_FAIL;*/

			if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppTexture[i])))
			{
				return E_FAIL;
			}

			if (wcswcs(szFullPath, L"EyeMouth.png"))
			{
				pDerivedMeshContainer->bMouse = TRUE;
			}

			if (wcswcs(szFullPath, L"Face.png"))
			{
				pDerivedMeshContainer->bFace = TRUE;
			}

			// 노말 텍스쳐 읽기
			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename,
				(_int)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename) - 4, szNormalName, 256);

			lstrcat(szNormalName, L"N.tga");

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szNormalName);
			/*	if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppNormalTexture[i])))
			{
				int temp;
				temp = 3;
			}*/
			if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppNormalTexture[i])))
			{
				int temp;
				temp = 3;
			}



			// Specular 텍스쳐 읽기
			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename,
				(_int)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename) - 4, szSpecularName, 256);

			lstrcat(szSpecularName, L"_Spec.png");

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szSpecularName);

			if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppSpecularTexture[i])))
			{
				int temp;
				temp = 3;
			}



			// Mask 텍스쳐 읽기
			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename,
				(_int)strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename) - 4, szMaskName, 256);

			lstrcat(szMaskName, L"_Mask.png");

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szMaskName);

			if (FAILED(CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppMaskTexture[i])))
			{
				int temp;
				temp = 3;

			}
		}
	}
	// 재질 정보가 없는 경우
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();

	
	DWORD dwNumBoneComboEntries = 0;
	pDerivedMeshContainer->dwMaxVertInfluences;

	D3DCAPS9 caps;
	m_pGraphicDev->GetDeviceCaps(&caps);
	int iApproxNumBones = caps.MaxVertexShaderConst / 4 - 4;

	_int iNumBones = pSkinInfo->GetNumBones();
	if (D3D_OK == pSkinInfo->ConvertToIndexedBlendedMesh(pMesh,
		D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		50,
		NULL,
		NULL,
		NULL,
		NULL,
		&pDerivedMeshContainer->dwMaxVertInfluences,
		&dwNumBoneComboEntries,
		&pDerivedMeshContainer->pBoneCombinationBuf,
		&pDerivedMeshContainer->MeshData.pMesh))
	{

	}

	pDerivedMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pDerivedMeshContainer->NumAttributeGroups);
	pDerivedMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pDerivedMeshContainer->NumAttributeGroups];
	pDerivedMeshContainer->MeshData.pMesh->GetAttributeTable(pDerivedMeshContainer->pAttributeTable, NULL);

	if (D3D_OK == pSkinInfo->ConvertToIndexedBlendedMesh(pMesh,
		D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
		50,
		NULL,
		NULL,
		NULL,
		NULL,
		&pDerivedMeshContainer->dwMaxVertInfluences,
		nullptr,
		nullptr,
		&pDerivedMeshContainer->pOriMesh))
	{
	}

	// 뼈의 개수를 반환
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffSetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffSetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppCombinedTransformMatrix = new _matrix*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppCombinedTransformMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	char szName[MAX_PATH][MAX_PATH];
	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
	{
		strcpy_s(szName[i], pDerivedMeshContainer->pSkinInfo->GetBoneName(i));

		pDerivedMeshContainer->pFrameOffSetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}

	*ppNewMeshContainer = pDerivedMeshContainer;

	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	Safe_Delete(pFrameToFree);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	/*for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
	{
		Safe_Release(pDerivedMeshContainer->ppTexture[i]);
		Safe_Release(pDerivedMeshContainer->ppNormalTexture[i]);
		Safe_Release(pDerivedMeshContainer->ppSpecularTexture[i]);
	}*/

	Safe_Delete_Array(pDerivedMeshContainer->ppTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppNormalTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppSpecularTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppMaskTexture);

	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);
	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);

	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffSetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->ppCombinedTransformMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderingMatrix);

	Safe_Delete_Array(pDerivedMeshContainer->pAttributeTable);


	Safe_Release(pDerivedMeshContainer->pDecl);


	// 문제생긴적 있음
	if (pDerivedMeshContainer->pBoneCombinationBuf)
	{
		Safe_Release(pDerivedMeshContainer->pBoneCombinationBuf);
		Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
		Safe_Release(pDerivedMeshContainer->pSkinInfo);
		Safe_Release(pDerivedMeshContainer->pOriMesh);
	}


	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

void CHierarchyLoader::Allocate_Name(char ** ppName, const char * pFrameName)
{
	if (nullptr == pFrameName)
	{
		return;
	}

	_uint	iLength = (_uint)strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}

_ulong CHierarchyLoader::Release()
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

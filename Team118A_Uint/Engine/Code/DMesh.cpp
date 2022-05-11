#include "DMesh.h"
#include "Transform.h"
#include "Renderer.h"
USING(Engine)

Engine::CDMesh::CDMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{
}

Engine::CDMesh::CDMesh(const CDMesh& rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
	, m_fAnimSpeed(1.f)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);

	//Safe_AddRef(m_pLoader);

	//_uint iNumMeshContainer = m_MeshContainerList.size();
	//auto iter = m_MeshContainerList.begin();
	//for (_uint i = 0; i < iNumMeshContainer; ++i,++iter)
	//{
	//	if (((*iter)->MeshData.pMesh))
	//	{
	//		((*iter)->MeshData.pMesh)->AddRef();
	//	}

	//	if (((*iter)->pOriMesh))
	//	{
	//		((*iter)->pOriMesh)->AddRef();
	//	}
	//	/*for (_uint j = 0; j < m_MeshContainers[i]->NumMaterials; ++j)
	//	{
	//		Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pDiffuseMap);
	//		Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pSpecularMap);
	//		Safe_AddRef(m_MeshContainers[i]->ppMaterialTextures[j]->pNormalMap);
	//	}*/
	//}
}

Engine::CDMesh::~CDMesh()
{
	
}

void CDMesh::Render_Meshes(CShader * pShader)
{
	
	//if (!pShader->IsDMesh()) 
	//{
	//	// DMesh 이여야지만 하드웨어 스키닝을 실행
	//	LMSG_BOX(L"하드웨어스키닝+쉐이더 관련 오류 문의:은호or민우");
	//	// DMesh 가 아니면 assert 를 한다.
	//	//assert(FALSE);
	//}

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();

	_int iBreakCount = 0;
	_int iContainerCount = 0;
	for (auto& iter : m_MeshContainerList)
	{

		++iBreakCount;
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;
		_int iNumBone = (_int)pDerivedMeshContainer->dwNumBones;
		LPD3DXBONECOMBINATION pBoneComb = nullptr;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pDerivedMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		if (!pBoneComb)
		{
			return;
		}

		for (_int i = 0; i < (_int)pDerivedMeshContainer->NumAttributeGroups; i++)
		{
			
			for (_int iPaletteEntry = 0; iPaletteEntry < 50; ++iPaletteEntry)
			{
				_int iMatrixIndex = pBoneComb[i].BoneId[iPaletteEntry];
				
				if (iMatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply(&pDerivedMeshContainer->pRenderingMatrix[iPaletteEntry], &pDerivedMeshContainer->pFrameOffSetMatrix[iMatrixIndex],m_CombinedTransfromationMatrixPointers[iContainerCount][iMatrixIndex]);
				}
			}

			_int iNumBones = (_int)pDerivedMeshContainer->dwNumBones;
			if (iNumBone <= 50)
			{
				pEffect->SetMatrixArray("g_Bone", pDerivedMeshContainer->pRenderingMatrix, iNumBones);
			}
			else
			{
				pEffect->SetMatrixArray("g_Bone", pDerivedMeshContainer->pRenderingMatrix, 50);
			}
			
			pEffect->SetInt("g_iNum", (int)pDerivedMeshContainer->dwMaxVertInfluences);
 			_int iMatIndex = pBoneComb[i].AttribId;

			//_bool bMouse = FALSE;
			//if (pDerivedMeshContainer->bMouse)
			//{
			//	bMouse = TRUE;
			//}
			//pEffect->SetBool("g_bMouse", bMouse);
			pEffect->SetBool("g_bMouse", pDerivedMeshContainer->bMouse);
			pEffect->SetBool("g_bFace", pDerivedMeshContainer->bFace);

			// 베이스 텍스쳐
			pEffect->SetTexture("g_BaseTexture", pDerivedMeshContainer->ppTexture[iMatIndex]);

			// 노말 텍스쳐
			if (pDerivedMeshContainer->ppNormalTexture[iMatIndex])
			{
				pEffect->SetTexture("g_NormalTexture", pDerivedMeshContainer->ppNormalTexture[iMatIndex]);
				pEffect->SetBool("g_bSpecularMap", TRUE);
			}
			else
			{
				pEffect->SetBool("g_bNormalMap", TRUE);
			}

			// 스펙큘러 텍스쳐
			if (pDerivedMeshContainer->ppSpecularTexture[iMatIndex])
			{
				pEffect->SetTexture("g_SpecTexture", pDerivedMeshContainer->ppSpecularTexture[iMatIndex]);
				pEffect->SetBool("g_bSpecMap", TRUE);
			}
			else
			{
				pEffect->SetBool("g_bSpecMap", FALSE);
			}

			//////////////////////////////////////////////////////////////////////////////
			// Glow		

			if (CRenderer::GetInstance()->Get_BaseGlow())
			{
				pEffect->SetFloat("g_fBaseGlow", pDerivedMeshContainer->fBaseGlow);
			}

			if (CRenderer::GetInstance()->Get_TexGlow() && pDerivedMeshContainer->ppMaskTexture[iMatIndex])
			{						
				pEffect->SetFloat("g_fTexGlowR", pDerivedMeshContainer->fTexGlowR);
				pEffect->SetFloat("g_fTexGlowG", pDerivedMeshContainer->fTexGlowG);
				pEffect->SetFloat("g_fTexGlowB", pDerivedMeshContainer->fTexGlowB);

				pEffect->SetTexture("g_MaskTexture", pDerivedMeshContainer->ppMaskTexture[iMatIndex]);
			}
			else
			{
				pEffect->SetFloat("g_fTexGlowR", 0.f);
				pEffect->SetFloat("g_fTexGlowG", 0.f);
				pEffect->SetFloat("g_fTexGlowB", 0.f);

				CRenderer::GetInstance()->Set_DefualtMaskTexture(pEffect, "g_MaskTexture");
			}

			pEffect->SetFloat("g_fGlow_U_Min", pDerivedMeshContainer->fGlow_U_Min);
			pEffect->SetFloat("g_fGlow_U_Max", pDerivedMeshContainer->fGlow_U_Max);
			pEffect->SetFloat("g_fGlow_V_Min", pDerivedMeshContainer->fGlow_V_Min);
			pEffect->SetFloat("g_fGlow_V_Max", pDerivedMeshContainer->fGlow_V_Max);
			
			pEffect->SetFloat("g_fGlowColorR", pDerivedMeshContainer->vGlowColor.x);
			pEffect->SetFloat("g_fGlowColorG", pDerivedMeshContainer->vGlowColor.y);
			pEffect->SetFloat("g_fGlowColorB", pDerivedMeshContainer->vGlowColor.z);

			pEffect->SetFloat("g_fGlowPower", pDerivedMeshContainer->fGlowPower);

			//////////////////////////////////////////////////////////////////////////////

			pEffect->CommitChanges();


			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);

		}
		++iContainerCount;
	}
}

void CDMesh::Render_Meshes_Instance(CShader * pShaderCom, vector<CGameObject*>& vecInstanceObj)
{
	//pSMesh->Get_Mesh()->GetIndexBuffer(&pIndexBuffer);
	//CGraphicDev::GetInstance()->GetDevice()->SetIndices(pIndexBuffer);
	

	LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();

	_int iIndex = 0;

	for (auto& iter : m_MeshContainerList)
	{
	
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;
		_int iNumBone = (_int)pDerivedMeshContainer->dwNumBones;
		LPD3DXBONECOMBINATION pBoneComb = nullptr;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pDerivedMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		if (!pBoneComb)
		{
			return;
		}

		LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = nullptr;
		pDerivedMeshContainer->MeshData.pMesh->GetVertexBuffer(&pVertexBuffer);
		CGraphicDev::GetInstance()->GetDevice()->SetStreamSource(0, pVertexBuffer, 0, sizeof(VTX_DMESH));
	
		LPDIRECT3DINDEXBUFFER9 pIndexBuffer = nullptr;
		pDerivedMeshContainer->MeshData.pMesh->GetIndexBuffer(&pIndexBuffer);
		CGraphicDev::GetInstance()->GetDevice()->SetIndices(pIndexBuffer);

		for (_int i = 0; i < (_int)pDerivedMeshContainer->NumAttributeGroups; i++)
		{
			LPDIRECT3DTEXTURE9 pAnimTex = Get_AnimTexture(iIndex, vecInstanceObj);
			pEffect->SetTexture("g_AnimTexture", pAnimTex);

			pEffect->SetInt("g_iNum", (int)pDerivedMeshContainer->dwMaxVertInfluences);
			_int iMatIndex = pBoneComb[i].AttribId;

			//_bool bMouse = FALSE;
			//if (pDerivedMeshContainer->bMouse)
			//{
			//	bMouse = TRUE;
			//}
			//pEffect->SetBool("g_bMouse", bMouse);
			pEffect->SetBool("g_bMouse", pDerivedMeshContainer->bMouse);
			pEffect->SetBool("g_bFace", pDerivedMeshContainer->bFace);

			// 베이스 텍스쳐
			pEffect->SetTexture("g_BaseTexture", pDerivedMeshContainer->ppTexture[iMatIndex]);

			// 노말 텍스쳐
			if (pDerivedMeshContainer->ppNormalTexture[iMatIndex])
			{
				pEffect->SetTexture("g_NormalTexture", pDerivedMeshContainer->ppNormalTexture[iMatIndex]);
				pEffect->SetBool("g_bSpecularMap", TRUE);
			}
			else
			{
				pEffect->SetBool("g_bNormalMap", TRUE);
			}

			// 스펙큘러 텍스쳐
			if (pDerivedMeshContainer->ppSpecularTexture[iMatIndex])
			{
				pEffect->SetTexture("g_SpecTexture", pDerivedMeshContainer->ppSpecularTexture[iMatIndex]);
				pEffect->SetBool("g_bSpecMap", TRUE);
			}
			else
			{
				pEffect->SetBool("g_bSpecMap", FALSE);
			}

			//////////////////////////////////////////////////////////////////////////////
			// Glow		

			if (CRenderer::GetInstance()->Get_BaseGlow())
			{
				pEffect->SetFloat("g_fBaseGlow", pDerivedMeshContainer->fBaseGlow);
			}

			if (CRenderer::GetInstance()->Get_TexGlow() && pDerivedMeshContainer->ppMaskTexture[iMatIndex])
			{
				pEffect->SetFloat("g_fTexGlowR", pDerivedMeshContainer->fTexGlowR);
				pEffect->SetFloat("g_fTexGlowG", pDerivedMeshContainer->fTexGlowG);
				pEffect->SetFloat("g_fTexGlowB", pDerivedMeshContainer->fTexGlowB);

				pEffect->SetTexture("g_MaskTexture", pDerivedMeshContainer->ppMaskTexture[iMatIndex]);
			}
			else
			{
				pEffect->SetFloat("g_fTexGlowR", 0.f);
				pEffect->SetFloat("g_fTexGlowG", 0.f);
				pEffect->SetFloat("g_fTexGlowB", 0.f);

				CRenderer::GetInstance()->Set_DefualtMaskTexture(pEffect, "g_MaskTexture");
			}

			pEffect->SetFloat("g_fGlow_U_Min", pDerivedMeshContainer->fGlow_U_Min);
			pEffect->SetFloat("g_fGlow_U_Max", pDerivedMeshContainer->fGlow_U_Max);
			pEffect->SetFloat("g_fGlow_V_Min", pDerivedMeshContainer->fGlow_V_Min);
			pEffect->SetFloat("g_fGlow_V_Max", pDerivedMeshContainer->fGlow_V_Max);

			pEffect->SetFloat("g_fGlowColorR", pDerivedMeshContainer->vGlowColor.x);
			pEffect->SetFloat("g_fGlowColorG", pDerivedMeshContainer->vGlowColor.y);
			pEffect->SetFloat("g_fGlowColorB", pDerivedMeshContainer->vGlowColor.z);

			pEffect->SetFloat("g_fGlowPower", pDerivedMeshContainer->fGlowPower);

			//////////////////////////////////////////////////////////////////////////////

			pEffect->CommitChanges();
			m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pDerivedMeshContainer->pAttributeTable[i].VertexCount, pDerivedMeshContainer->pAttributeTable[i].FaceStart * 3, pDerivedMeshContainer->pAttributeTable[i].FaceCount);

			++iIndex;
			Safe_Release(pAnimTex);
		}
		
	}
}

LPDIRECT3DTEXTURE9 CDMesh::Get_AnimTexture(_int iIndex, vector<CGameObject*>& vecInstanceObj)
{
	_matrix matAnim[50];
	
	LPDIRECT3DTEXTURE9 pAnimTex;

	D3DXCreateTexture(m_pGraphicDev,
		256,
		256,
		1,  // miplevel
		0,	// 텍스처의 용도, D3DUSAGE_RENDERTARGET
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&pAnimTex);

	D3DLOCKED_RECT LockRect;
	pAnimTex->LockRect(0, &LockRect, NULL, 0);

	_int iObjIndex = 0;
	for (auto& pObj : vecInstanceObj)
	{
		if (pObj->Get_Active())
		{
			CDMesh* pDMesh = static_cast<CDMesh*>(pObj->Get_Component(ComTag_DMesh));
			if (pDMesh)
			{
				pDMesh->Get_CombineMatrixArray(iIndex, matAnim);
			}

			
			memcpy(((_ulong*)LockRect.pBits + (iObjIndex * 200)), matAnim, sizeof(_matrix) * 50);
			
			++iObjIndex;
		}

	
	}


	pAnimTex->UnlockRect(0);

	return pAnimTex;
}

void CDMesh::Get_CombineMatrixArray(_int iArrayIndex, _matrix * pArrayMatrix)
{

	_int iIndex = 0;
	_int iContainerCount = 0;
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;
		_int iNumBone = (_int)pDerivedMeshContainer->dwNumBones;
		LPD3DXBONECOMBINATION pBoneComb = nullptr;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pDerivedMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		if (!pBoneComb)
		{
			return;
		}

		for (_int i = 0; i < (_int)pDerivedMeshContainer->NumAttributeGroups; i++)
		{
			if (iIndex != iArrayIndex)
			{
				++iIndex;
				continue;
			}
			else
			{
				for (_int iPaletteEntry = 0; iPaletteEntry < 50; ++iPaletteEntry)
				{
					_int iMatrixIndex = pBoneComb[i].BoneId[iPaletteEntry];

					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&pArrayMatrix[iPaletteEntry], &pDerivedMeshContainer->pFrameOffSetMatrix[iMatrixIndex], m_CombinedTransfromationMatrixPointers[iContainerCount][iMatrixIndex]);
					}
				}
				return;
			}
		}
		
		++iContainerCount;
	}
}

HRESULT CDMesh::Copy_Bone()
{

	/* 1. 애니메이션이 재생될경우 Transformation행렬이 갱신. PlayAnimation함수를 봐. */
	/* 2. UpdasteCombinedTransform함술르 호출해서 컴바인드 행렬을 갱신. */
	/* 3. 복제된 객체수만큼의 뼈의 정보들이필요햇다. */



	/* 컴바인드행렬의 주소를 담기위한 배열을 메시컨테이너 갯수만큼 확보한다. */
	m_CombinedTransfromationMatrixPointers.resize(m_MeshContainerList.size());

	/* 뼈를 복제한다. */
	D3DXFRAME_DERIVED*		pOriginalFrame = (D3DXFRAME_DERIVED*)m_pRootFrame;

	/* m_pRootFrame에 원본뼈를 복사하여 담는다.  */
	if (FAILED(Clone_Frame(pOriginalFrame)))
		return E_FAIL;

	/* 렌더링할때 UpdateSkinnedMesh함수를 호출한다. */
	/* UpdateSkinnedMesh : 내가 그릴려고하는 메시(정점으로이루어져있다.)에 뼈들의 컴바인드 행렬의 상태를 곱하여. 정점들을 모두 뼈의 상태로 갱신하다. */
	/* 뼈의 컴바인드를 찾아서 가져오는 형태가 아니었다고. 로드시에 현재 메시에 영향을 미치는 뼏르의 컴바인드행렬의 주소를 담아놓는 배열을 미리 만들고. */
	/* UpdateSkinnedMesh에서 저장해놓은 주소를 따라가서 행렬을 가져오는형태 .*/
	/* 그 뼈들이 메시_다이나믹마다 각 각 존재했다. */
	/* 각각보관하기위한 작업이 필요했다. */
	/* 그래서 이변수를 메시_다이나믹에다가 각각 선언해놓은거야. */
	/* 공간할당한거고. */

	auto iter = m_MeshContainerList.begin();
	
	for (_uint i = 0; i < m_MeshContainerList.size(); ++i,++iter)
	{
		m_CombinedTransfromationMatrixPointers[i] = new _matrix*[(*iter)->dwNumBones];
	}

	_uint		iIndex = 0;

	/* 이게 그거야. */
	/* 현재 메시에 영햐을 미치는 뼈들의 컴바인드행렬의 주소를 찾아서 위에서 확보해놓은 배열에 주소를 담아놓는다. */
	SetUp_CloneCombinedTransformationMatricesPointer(m_pRootFrame, iIndex);

	/* */
	m_pAniCtrl->Bind_Frames((D3DXFRAME_DERIVED*)m_pRootFrame);
	
	return S_OK;
}

void CDMesh::Clear_Bone()
{
	/*_uint iNumMeshContainer = m_MeshContainerList.size();
	auto iter = m_MeshContainerList.begin();
	for (_uint i = 0; i < iNumMeshContainer; ++i,++iter)
	{
		
		Safe_Release((*iter)->MeshData.pMesh);
		Safe_Release((*iter)->pOriMesh);
		for (_uint j = 0; j < (*iter)->NumMaterials; ++j)
		{
			Safe_Release((*iter)->ppTexture[j]);
			Safe_Release((*iter)->ppNormalTexture[j]);
			Safe_Release((*iter)->ppSpecularTexture[j]);
		}
	}*/

	//m_MeshContainerList.clear();

	//m_pLoader->DestroyFrame(m_pRootFrame);
	
	Clear_Frame(static_cast<D3DXFRAME_DERIVED*>(m_pRootFrame));
	for (auto& ppCombinedMatrixPointers : m_CombinedTransfromationMatrixPointers)
		Safe_Delete_Array(ppCombinedMatrixPointers);
	m_CombinedTransfromationMatrixPointers.clear();

	/*Safe_Release(m_pLoader);

	Safe_Release(m_pAniCtrl);*/
}

void CDMesh::Set_Glow_Option(_int iContainerIndex, _float bBaseGlow, _vec3 vTexGlowRGB, _float bGlowPower, _float fGlow_U_Min, _float fGlow_U_Max, _float fGlow_V_Min, _float fGlow_V_Max, _vec3 vGlowColor/*=_vec3(1.f, 1.f, 1.f)*/)
{
	auto iter = m_MeshContainerList.begin();

	for (_int i = 0; i < iContainerIndex; i++)
	{
		iter++;
	}		
		
	(*iter)->fBaseGlow	= bBaseGlow;
	(*iter)->fTexGlowR = vTexGlowRGB.x;
	(*iter)->fTexGlowG = vTexGlowRGB.y;
	(*iter)->fTexGlowB = vTexGlowRGB.z;
	(*iter)->fGlowPower	= bGlowPower;

	(*iter)->fGlow_U_Min = fGlow_U_Min;
	(*iter)->fGlow_U_Max = fGlow_U_Max;
	(*iter)->fGlow_V_Min = fGlow_V_Min;
	(*iter)->fGlow_V_Max = fGlow_V_Max;

	(*iter)->vGlowColor = vGlowColor;
}

void CDMesh::Set_Glow_Option(_int iContainerIndex, T_GlowOption tGlowOption)
{
	auto iter = m_MeshContainerList.begin();

	for (_int i = 0; i < iContainerIndex; i++)
	{
		iter++;
	}

	(*iter)->fBaseGlow = tGlowOption.fBaseGlow;
	(*iter)->fTexGlowR = tGlowOption.fTexGlowR;
	(*iter)->fTexGlowG = tGlowOption.fTexGlowG;
	(*iter)->fTexGlowB = tGlowOption.fTexGlowB;
	(*iter)->fGlowPower = tGlowOption.fGlowPower;

	(*iter)->fGlow_U_Min = tGlowOption.fGlow_U_Min;
	(*iter)->fGlow_U_Max = tGlowOption.fGlow_U_Max;
	(*iter)->fGlow_V_Min = tGlowOption.fGlow_V_Min;
	(*iter)->fGlow_V_Max = tGlowOption.fGlow_V_Max;

	(*iter)->vGlowColor = tGlowOption.vGlowColor;
}

T_GlowOption CDMesh::Get_Glow_Option(_int iContainerIndex)
{
	T_GlowOption tGlowOption = T_GlowOption();

	auto iter = m_MeshContainerList.begin();

	for (_int i = 0; i < iContainerIndex; i++)
	{
		iter++;
	}
	
	tGlowOption.fBaseGlow	= (*iter)->fBaseGlow	;
	tGlowOption.fTexGlowR	= (*iter)->fTexGlowR	;
	tGlowOption.fTexGlowG	= (*iter)->fTexGlowG	;
	tGlowOption.fTexGlowB	= (*iter)->fTexGlowB	;
	tGlowOption.fGlowPower	= (*iter)->fGlowPower	;

	tGlowOption.fGlow_U_Min	= (*iter)->fGlow_U_Min	;
	tGlowOption.fGlow_U_Max	= (*iter)->fGlow_U_Max	;
	tGlowOption.fGlow_V_Min	= (*iter)->fGlow_V_Min	;
	tGlowOption.fGlow_V_Max	= (*iter)->fGlow_V_Max	;

	tGlowOption.vGlowColor	= (*iter)->vGlowColor	;

	return tGlowOption;
}

HRESULT CDMesh::Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame)
{
	m_pRootFrame = new D3DXFRAME_DERIVED;
	memcpy(m_pRootFrame, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

	((D3DXFRAME_DERIVED*)m_pRootFrame)->isCloned= true;

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		m_pRootFrame->pFrameFirstChild = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)m_pRootFrame->pFrameFirstChild);
	}

	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		m_pRootFrame->pFrameSibling = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling, (D3DXFRAME_DERIVED*)m_pRootFrame->pFrameSibling);
	}

	return S_OK;
}

void CDMesh::Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame, D3DXFRAME_DERIVED * pOut)
{
	memcpy(pOut, pOriginalFrame, sizeof(D3DXFRAME_DERIVED));

	pOut->isCloned = true;

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		pOut->pFrameFirstChild = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild, (D3DXFRAME_DERIVED*)pOut->pFrameFirstChild);
	}

	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		pOut->pFrameSibling = new D3DXFRAME_DERIVED;
		Clone_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling, (D3DXFRAME_DERIVED*)pOut->pFrameSibling);
	}

}

HRESULT CDMesh::Clear_Frame(D3DXFRAME_DERIVED * pOriginalFrame)
{
	
	

	if (nullptr != pOriginalFrame->pFrameFirstChild)
	{
		Clear_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameFirstChild);
	}

	if (nullptr != pOriginalFrame->pFrameSibling)
	{
		Clear_Frame((D3DXFRAME_DERIVED*)pOriginalFrame->pFrameSibling);
	}

	Safe_Delete(pOriginalFrame);
	return S_OK;
}

void CDMesh::SetUp_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint & iIndex)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer_Derived = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_uint i = 0; i < pMeshContainer_Derived->dwNumBones; ++i)
		{
			/* 현재 메시에 영향을 미치는 뼈들 중, i번째 뼈의 이름을 리턴해주낟. */
			const char* pBoneName = pMeshContainer_Derived->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED*		pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			
			m_CombinedTransfromationMatrixPointers[iIndex][i] = &pFindFrame->CombinedTransformMatrix;
			// pMeshContainer_Derived->ppCombindTransformationMatrices[i] = &pFindFrame->CombinedTransformationMatrix;
		}

		++iIndex;
	}



	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CloneCombinedTransformationMatricesPointer(pFrame->pFrameFirstChild, iIndex);

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CloneCombinedTransformationMatricesPointer(pFrame->pFrameSibling, iIndex);
}

void CDMesh::Set_AnimationIndex_Loop(const _uint & iIndex)
{
	m_pAniCtrl->Set_AnimationIndex_Loop(iIndex); 
}

void CDMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta * m_fAnimSpeed);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
}

Engine::_bool Engine::CDMesh::Is_AnimationsetFinish()
{
	return m_pAniCtrl->Is_AnimationsetFinish();
}

_uint CDMesh::Get_AnimCountMax()
{
	return m_pAniCtrl->Get_AnimCountMax();
}

_uint CDMesh::Get_OldAniIndex()
{
	return m_pAniCtrl->Get_OldAniIndex();
}

D3DXFRAME * CDMesh::Get_Frame()
{
	return m_pRootFrame;
}

list<D3DXMESHCONTAINER_DERIVED*>& CDMesh::Get_MeshContainerList()
{
	return m_MeshContainerList;
}

void CDMesh::Set_AnimSpeed(_float fAnimSpeed)
{
	if (fAnimSpeed < 0.f)
	{
		m_fAnimSpeed = 1.f;
	}
	else
	{
		m_fAnimSpeed = fAnimSpeed;
	}
}

void CDMesh::Set_AnimaRatio(_float fRatio)
{
	m_pAniCtrl->Set_AnimaRatioFor_Stop();
	m_pAniCtrl->Play_Animation(fRatio);
	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
}

const _float CDMesh::Get_AnimRatio()
{
	return m_pAniCtrl->Get_AnimRatio();
}

const _float CDMesh::Get_AnimMaxTime()
{
	return m_pAniCtrl->Get_AnimMaxTime();
}

const _int CDMesh::Get_AnimCurrentFrame()
{
	return m_pAniCtrl->Get_AnimCurrentFrame();
}

const _int CDMesh::Get_AnimMaxFrame()
{
	return m_pAniCtrl->Get_AnimMaxFrame();
}

_float CDMesh::Get_CurAniTime()
{
	return m_pAniCtrl->Get_CurAniTime();
}

_float CDMesh::Get_CurAniEndTime()
{
	return m_pAniCtrl->Get_CurAniEndTime();
}

const	Engine::D3DXFRAME_DERIVED* Engine::CDMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}


HRESULT Engine::CDMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader, // HierarchyLoader
		NULL,
		&m_pRootFrame,
		&pAniCtrl)))	// AniCtrl
		return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);

	_matrix		matTemp;

	SetUp_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;

}

void Engine::CDMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

CDMesh * CDMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar * pFileName)
{
	CDMesh*	pInstance = new CDMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CDMesh::Clone()
{
	CDMesh* pInstance = new CDMesh(*this);

	if (FAILED(pInstance->Copy_Bone()))
	{
		LMSG_BOX(L"Failed to Copy Bone");
		assert(FALSE);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CDMesh::Free()
{
	Safe_Release(m_pAniCtrl);

 	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	for (auto& iter : m_pListMesh)
	{
		Safe_Release(iter);
	}
	m_pListMesh.clear();

	

	CComponent::Free();
}

void Engine::CDMesh::SetUp_FrameMatrices(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED* pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

			pDerivedMeshContainer->ppCombinedTransformMatrix[i] = &pDerivedFrame->CombinedTransformMatrix;
		}

		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}


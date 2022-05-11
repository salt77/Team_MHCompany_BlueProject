#ifndef __DYNAMICMESH_H__
#define __DYNAMICMESH_H__

#include "Engine_Define.h"
#include "Component.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"
#include "Shader.h"

BEGIN(Engine)

// 파일 가져온 사람 : 김민현

class CTransform;
class CGameObject;

class ENGINE_DLL CDMesh : public CComponent
{
private:
	explicit CDMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDMesh(const CDMesh& rhs);
	virtual ~CDMesh();

public:
	virtual CComponent*					Clone();
	virtual void						Free();
	static CDMesh*						Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);

public:
	HRESULT								Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	
	void								Render_Meshes(CShader* pShader);
	void								Render_Meshes_Instance(CShader* pShaderCom, vector<CGameObject*>& vecInstanceObj);

	LPDIRECT3DTEXTURE9					Get_AnimTexture(_int iIndex, vector<CGameObject*>& vecInstanceObj);
	void								Get_CombineMatrixArray(_int iArrayIndex, _matrix* pArrayMatrix);

	HRESULT								Copy_Bone();

	void								Clear_Bone();
	
	void								Set_Glow_Option(_int iContainerIndex, _float bBaseGlow, _vec3 vTexGlowRGB, _float bGlowPower, _float fGlow_U_Min, _float fGlow_U_Max, _float fGlow_V_Min, _float fGlow_V_Max, _vec3 vGlowColor = _vec3(1.f, 1.f, 1.f));
	void								Set_Glow_Option(_int iContainerIndex, T_GlowOption tGlowOption);

	// DMesh는 구조체를 받아와 Set 해줘야함 / Smesh 와 차이가 있음
	T_GlowOption						Get_Glow_Option(_int iContainerIndex);

public:
	void								Set_AnimationIndex_Loop(const _uint& iIndex);
	void								Play_Animation(const _float& fTimeDelta);
	const								D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	_bool								Is_AnimationsetFinish();
	_uint								Get_AnimCountMax();
	CAniCtrl*							Get_AniCtrl() { return m_pAniCtrl; }
	_uint								Get_OldAniIndex();
	D3DXFRAME*							Get_Frame();
	list<D3DXMESHCONTAINER_DERIVED*>&	Get_MeshContainerList();
	void								Set_AnimSpeed(_float fAnimSpeed);
	void								Set_AnimaRatio(_float fRatio);
	const _float						Get_AnimRatio();
	const _float						Get_AnimMaxTime();
	const _int							Get_AnimCurrentFrame();
	const _int							Get_AnimMaxFrame();		
	_float								Get_CurAniTime();
	_float								Get_CurAniEndTime();

public:
	// 모든 뼈들을 재귀적으로 순회하면서 부모와 자식, 또는 형제 관계를 맺을 수 있도록 행렬들을 곱하여 완성된 월드 행렬 상태를 만들어주는 함수
	void								Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void								SetUp_FrameMatrices(D3DXFRAME_DERIVED* pFrame);
	D3DXFRAME*							Get_RootFrame() { return m_pRootFrame; }
private:
	HRESULT								Clear_Frame(D3DXFRAME_DERIVED * pOriginalFrame);
	HRESULT								Clone_Frame(D3DXFRAME_DERIVED* pOriginalFrame);
	void								Clone_Frame(D3DXFRAME_DERIVED * pOriginalFrame, D3DXFRAME_DERIVED* pOut);
	void								SetUp_CloneCombinedTransformationMatricesPointer(LPD3DXFRAME pFrame, _uint& iIndex);
private:

	D3DXFRAME*							m_pRootFrame;
	CHierarchyLoader*					m_pLoader;
	list<D3DXMESHCONTAINER_DERIVED*>	m_MeshContainerList;
	CAniCtrl*							m_pAniCtrl;
	_float								m_fAnimSpeed = 1.f;

	vector<_matrix**>		m_CombinedTransfromationMatrixPointers;

public:
	list<_vec3>							m_pListTrailVtx;
	list<_matrix>						m_pCombinedTransformMatrix;
	list<LPD3DXMESH>					m_pListMesh;
	

};

END

#endif // __DYNAMICMESH_H__
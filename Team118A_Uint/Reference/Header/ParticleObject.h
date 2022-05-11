#pragma once

#ifndef __PARTICLE_OBJECT_H__
#define __PARTICLE_OBJECT_H__

#include "GameObject.h"

BEGIN(Engine)

class CCrossRectBuffer;
class CRectBuffer;

class ENGINE_DLL CParticleObject : public CGameObject
{
private:
	CParticleObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CParticleObject(const CParticleObject& rhs);
	virtual ~CParticleObject();
public:
	virtual void	Free()override;
	
	virtual	HRESULT	Ready_Object(wstring wstrParticleTag = L"");
	virtual _int	Start_Object(const _float& fTimeDelta);
	virtual	_int	Update_Object(const _float& fTimeDelta);
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta);
	virtual	void	Render_Object(_int iPass = 0);
	virtual	void	Render_Shadow();

	virtual void	Save(HANDLE hFile, DWORD& dwByte);
	virtual void	Load(HANDLE hFile, DWORD& dwByte);

	virtual void	Set_Constantable()override;
	virtual void	Debug_This_In_Console();

	virtual void	Set_Scale(_vec3 vScale) { m_pTransformCom->Set_Scale(vScale * m_fStartScale); }
	virtual void	Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ) { m_pTransformCom->Set_Scale(fScaleX * m_fStartScale, fScaleY * m_fStartScale, fScaleZ * m_fStartScale); }
	virtual void	Set_Scale(E_SCALE eScale, _float fScale) { m_pTransformCom->Set_Scale(eScale, fScale * m_fStartScale); }

	void			Set_ShowWindow(_bool bShowWindow) { m_bShowWindow = TRUE; }
public:
	void	Set_ParticleDead();

	void	PrefabLoad(wstring wstrPrefabName);

	void	Add_ParticleSample(T_PARTICLE tParticleSample);
	void	Add_WaitSpawner(T_PARTICLE_SPAWNER tSpawner);
	void	Add_Texture(wstring wstrTexturePath);

	void	Delete_Texture(_int iIndex);
	void	Delete_ParticleSample(_int iIndex);
	void	Delete_WaitSpawner(_int iIndex);


	_int	Get_ListParticleSampleSize();
	_int	Get_ListWaitSpawnerSize();
	_int	Get_ListActiveSpawnerSize();
	
	_int	Get_vecTextureSize();

	T_PARTICLE*	Get_ParticleSamplePtr(_int iIndex);
	T_PARTICLE_SPAWNER*	Get_WaitSpawnerPtr(_int iIndex);
	T_PARTICLE_SPAWNER* Get_ActiveSpawnerPtr(_int iIndex);
	wstring Get_TexturePath(_int iIndex);
	
	void	Set_LifeTime(_float fLifeTime);
	_float	Get_LifeTime();

private:
	void				Add_Component();

	void				WaitSpawnerCheck();
	void				SpawnerUpdate(_float fDeltaTime);
	void				ParticleUpdate(_float fDeltaTime);
	void				SetInstanceBuffer();

	void				AbsoluteSpawnerUpdate(T_PARTICLE_SPAWNER* pSpawner, _float fDeltaTime);
	void				LocalSpawnerUpdate(T_PARTICLE_SPAWNER* pSpawner, _float fDeltaTime);
	void				LocalNoneSpawnerUpdate(T_PARTICLE_SPAWNER* pSpawner, _float fDeltaTime);

	void				AbsoluteParticleUpdate(T_PARTICLE* pParticle, _float fDeltaTime);
	void				LocalParticleUpdate(T_PARTICLE* pParticle, _float fDeltaTime);
	void				LocalNoneParticleUpdate(T_PARTICLE* pParticle, _float fDeltaTime);
	void				CenterParticleUpdate(T_PARTICLE* pParticle, _float fDeltaTime);

	void				ParticleSpriteUpdate(T_PARTICLE* pParticle, _float fDeltaTime);

	void				ParticleGravityUpdate(T_PARTICLE* pParticle, _float fDeltaTime);
	void				ParticleColorUpdate(T_PARTICLE* pParticle);
	void				ParticleScaleUpdate(T_PARTICLE* pParticle);
	_vec3				GetBillBoardRotation(_vec3 vPos);
	_vec3				GetRandomVector(_vec3 vMin, _vec3 vMax);
	_vec4				GetRandomVector4(_vec4 vMin, _vec4 vMax);
	void				ParticleSpawn(_vec3 vPos,_vec3 vRot,_vec3 vMinSpawnPos,_vec3 vMaxSpawnPos, _int iSampleIndex, _vec3 vSpawnerLook, _vec3 vSpawnerRight, _vec3 vSpawnerUp);

private:
	list<T_PARTICLE_SPAWNER> m_listWaitSpawner;

	list<T_PARTICLE_SPAWNER> m_listActiveSpawner;
	vector<list<T_PARTICLE>> m_vecListParticle;

	vector<T_PARTICLE>m_vecParticleSample;
	
	vector<wstring> m_vecTexturePath;

	vector<LPDIRECT3DTEXTURE9> m_vecTexture;
	CRectBuffer* m_pRectBuffer = nullptr;
	CCrossRectBuffer* m_pCrossRectBuffer = nullptr;
	CShader*	m_pShaderCom = nullptr;
	
	wstring		m_wstrParticleTag;

	LPDIRECT3DVERTEXDECLARATION9	m_pDecl = nullptr;
	vector<LPDIRECT3DVERTEXBUFFER9>	m_vecInstanceBuffer;

	_uint							m_iObjectCount = 0;	
	_uint							m_iVertexSize  = 0;

	_float							m_fStartTime = 0.f;
	_float							m_fLifeTime = 5.f;

	_int							iTemp[4];
	_bool							bTemp[5];
	_float							m_fStartScale = 1.f;

	_bool							m_bShowWindow = FALSE;
public:
	static CParticleObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrParticleTag);
	static CParticleObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);

};

END

#endif
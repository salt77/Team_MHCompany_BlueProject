#pragma once
#ifndef __MASCOT_H__
#define __MASCOT_H__

#include "GameObject.h"
#include "SphereCollider.h"

#include "DMesh.h"

#include "ProtoMgr.h"

#include "Renderer.h"
#include "ProtoMgr.h"
#include "LightMgr.h"
#include "FogManager.h"
#include "KeyManager.h"
#include "CharacterMgr.h"
#include "WorldCreator.h"

BEGIN(Engine)

class CTexture;
class ENGINE_DLL CMascot : public CGameObject
{
protected:
	explicit CMascot(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrWorldName);
	explicit CMascot(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_SQUAD eSquad, wstring wstrUnitName);
	explicit CMascot(const CMascot& rhs);
	virtual ~CMascot();

public:
	static	CMascot* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrWorldName);
	static	CMascot* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_SQUAD eSquad, wstring wstrUnitName);

	virtual void	Free()								override;
	virtual void	Save(HANDLE hFile, DWORD& dwByte)	override;
	virtual void	Load(HANDLE hFile, DWORD& dwByte)	override;
	void			Load_Animation();

public:		/* 기본		*/
	virtual	HRESULT	Ready_Object()									override;
	virtual _int	Start_Object(const _float& fTimeDelta)			override;
	virtual	_int	Update_Object(const _float& fTimeDelta)			override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual	void	Render_Object(_int iPass)						override;
	virtual void	Debug_This_In_Console()							override;
	virtual void	Set_Constantable()								override;
	void			UIReady();
public:		/* 이벤트	*/
	virtual void			OnEnable()							override;
	virtual void			OnDisable()							override;
	virtual void			OnDestroy()							override;

public:		/* public 함수 */
	virtual void			Animation_Change(const _float& fTimeDelta);
	virtual void			Reset();

	E_SQUAD					Get_Squad() { return m_eSquad; }
public:		/* Get		*/
	virtual const wstring			Get_Name() override					{ return m_wstrUnitName;										}
	E_ENEMY_PLAY_TYPE				Get_PlayType()						{ return m_ePlayType; }
	wstring							Get_WorldName()						{ return m_wstrWorldName; }
	CWorldCreator*					Get_WorldCreator()					{ return m_pWorldCreater; }
	_int							Get_CurrentTileIndex()				{ return m_iCurrentTileIndex; }

	const vector<T_MONSTER_INFO>&	Get_MonsterPoolInfo()				{ return m_vecMonsterPool; }
public:		/* Set		*/
	void							Set_PlayType(E_ENEMY_PLAY_TYPE ePlayType) { m_ePlayType = ePlayType; UIReady(); }
	void							Set_CurrentTileIndex(_int iIndex) { m_iCurrentTileIndex = iIndex; }

	void							Set_ActionByEnum(E_ACTION eAction);
	void							Set_BodyBoneName(char* szName) { m_szBodyBoneName = szName; }

	void							Set_AP(_int iAP) { m_iAP = iAP; }
	_int							Get_AP() { return m_iAP; }

	_bool							Get_Move() { return m_bMove; }
	void							Set_MovePos(_vec3 vTargetPos);

	_bool							Get_MascotDead() { return m_bMascotDead; }
	void							Set_MascotDead(_bool bDead) { m_bMascotDead = bDead; }

	_int							Get_WarpPoint() { return m_iWarpPoint; }
	void							Set_WarpPoint(_int iWarpPoint) { m_iWarpPoint = iWarpPoint; }

	_int							Get_StartIndex() { return m_iStartIndex; }
	void							Set_StartIndex(_int iStartIndex) { m_iStartIndex = iStartIndex; }

	_bool							Get_Boss() { return m_bBoss; }

	void							SetUIActive(_bool bActive);
protected:	/* 액션 트리거	*/
	void			Initialize_Trigger();
	void			ReSetting_AnimActionVec();
	void			ReSetting_ActionTrigger();

	void			Active_Trigger(T_AnimTrg& trg);
	virtual void	Trigger_Attack_1()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Attack1");		}
	virtual void	Trigger_Attack_2()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Attack2");		}
	virtual void	Trigger_Attack_3()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Attack3");		}
	virtual void	Trigger_Dead()			{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Dead");		}
	virtual void	Trigger_Reload()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Reload");		}
	virtual void	Trigger_ExSkill()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 ExSkill");		}
	virtual void	Trigger_Buff_1()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Buff1");		}
	virtual void	Trigger_Buff_2()		{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 Buff2");		}
	virtual void	Trigger_1()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 1" );			}
	virtual void	Trigger_2()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 2" );			}
	virtual void	Trigger_3()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 3" );			}
	virtual void	Trigger_4()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 4" );			}
	virtual void	Trigger_5()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 5" );			}
	virtual void	Trigger_6()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 6" );			}
	virtual void	Trigger_7()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 7" );			}
	virtual void	Trigger_8()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 8" );			}
	virtual void	Trigger_9()				{	CUtility::Print_Wstring_DMode(L"실행 - 트리거 9" );			}


protected:	/* protected 함수 */

	virtual void				Ready_DMesh();
	_bool						Find_DMesh();

	_bool						Is_AnimFinished();

	void						Update_Trigger(const _float& fTimeDelta);
	void						Update_Collider(const _float& fTimeDelta);

	void						Update_FSM(_float fTimeDelta);
	void						Move(_float fTimeDelta);
private:
	void			Set_AnimIndexForTrigger(_uint iIndex);
	void			Set_ActionByName(wstring wstrName);

	
	void			UIUpdate(_float fTimeDelta);
	void			UILateUpdate(_float fTimeDelta);

protected:  
	CUI_Object*			m_pPlayTypeUI = nullptr;
	CUI_Object*			m_pRankUI = nullptr;

	_bool				m_bMascotDead = FALSE;
	_bool				m_bMove = FALSE;
	_bool				m_bBoss = FALSE;

	_vec3				m_vMovePos;

	E_SQUAD				m_eSquad				= E_SQUAD::SQUAD_END;
	CDMesh*				m_pDMeshCom				= nullptr;
	CShader*			m_pShaderCom			= nullptr;
	CSphereCollider*	m_pSphereCollider		= nullptr;				

	wstring				m_wstrUnitName			= L"Empty";
	wstring				m_wstrDMeshProtoTag		= L"Empty";
	wstring				m_wstrDMeshFullPath		= L"Empty";
	char*				m_szBodyBoneName		= "Bip001_Pelvis";			// 몸통 뼈 위치 보통은 Bip001 Pelvis
	

	E_ACTION			m_ePrevAction			= E_ACTION::ACTION_END;
	E_ACTION			m_eCurrentAction		= E_ACTION::ACTION_END;


	_float				m_fDestDistForArrive	= 0.05f;					// 이동 최소 단위
	_float				m_fMoveSpeed			= 1.8f;						// 이동속도
	_float				m_fDeadShaderFilter = 0.f;

	E_ENEMY_PLAY_TYPE	m_ePlayType = E_ENEMY_PLAY_TYPE::TYPE_DEFENSIVE;
	wstring             m_wstrWorldName = L"";
	CWorldCreator*		m_pWorldCreater = nullptr;

	_int				m_iCurrentTileIndex = 0;

	_int				m_iAP = 0;
	_int				m_iWarpPoint = 0;

	_int				m_iStartIndex = 0;
	vector<T_MONSTER_INFO> m_vecMonsterPool;
protected:	/* 액션 트리거	*/
	vector<pair<E_Trigger, std::function<void()>>>	m_vecFunction_Trg;
	vector<list<T_AnimTrg>>							m_vecAnimTrg;
	vector<T_AnimActionSys>							m_vecAnimActions;
	queue<_int>										m_qNextAnimIdx;
	queue<T_AnimTrg>								m_qTrg;






};

END

#endif // !__UNIT_H__
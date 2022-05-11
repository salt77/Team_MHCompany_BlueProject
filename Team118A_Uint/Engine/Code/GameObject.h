#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "GraphicDev.h"
#include "Component.h"
#include "Base.h"
#include "Transform.h"
#include "Shader.h"

BEGIN(Engine)

class CDMesh;
class CSMesh;

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	virtual	HRESULT	Ready_Object();
	virtual _int	Start_Object(const _float& fTimeDelta);
	virtual	_int	Update_Object(const _float& fTimeDelta);
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta);

	virtual	void	Render_Object(_int iPass = 0) {};

	virtual void	Free() override;
	virtual void	Save(HANDLE hFile, DWORD& dwByte);
	virtual void	Load(HANDLE hFile, DWORD& dwByte);

	virtual void	Debug_This_In_Console();

	virtual void	Set_Constantable() {};
	virtual void	Set_Constantable_Fog(CShader* pShader);

	virtual CTransform* Get_Transform();

	CComponent*		Get_Component(const wstring& wstrComponentTag);
	CComponent*		Emplace_Component(const wstring& wstrComponentTag, CComponent* pComponent);
	void			Delete_Component(const wstring& wstrComponentTag);
	void			Replace_Component(const wstring& wstrComponentTag, CComponent* pComponent);

public:
	// �� �Լ��� ������Ʈ�� Ȱ��ȭ �� ���Ŀ� ȣ��˴ϴ�. (Active True)
	virtual void	OnEnable();
	// �� �Լ��� ������ ��Ȱ��ȭ �� �� ȣ��˴ϴ�. (Active False)
	virtual void	OnDisable();
	// ������Ʈ �����Ⱓ�� ������ �������� ������ ������Ʈ �� OnDestroy�� ȣ��˴ϴ�
	virtual void	OnDestroy();
	virtual void	On_Collision(CGameObject* pDest);

public:
	virtual void	Set_Dead(_bool bState)				{ m_bDead = bState;				}
	void			Set_Active(_bool bState);
	void			Set_PassNumber(_int iNum)			{ m_iPassNumber = iNum;			}
	void			Set_LayerTag(E_LAYERTAG eLayerTag)	{ m_eMyLayerTag = eLayerTag;	}

public:
	virtual const wstring	Get_Name()	{ return to_wstring((_int)m_eObjTag);	}
	const _bool&	Get_Start()			{ return m_bStart;						}
	const _bool&	Get_Dead()			{ return m_bDead;						}
	const _bool&	Get_Active()		{ return m_bActive;						}
	const E_OBJTAG&	Get_ObjTag()		{ return m_eObjTag;						}
	const E_LAYERTAG&	Get_LayerTag()	{ return m_eMyLayerTag;					}

	_float			Get_ViewZ()			{ return m_fViewZ;	}
	virtual _int	Get_SortingLayer()	{ return 0;			};		// Renderer���� Ui Object - Sort �ϱ����� �뵵
	virtual _int	Get_OrderInLayer()	{ return 0;			};		// Renderer���� Ui Object - Sort �ϱ����� �뵵
	void			Compute_ViewZ(const _vec3* pPos);

public: ///////////////////////////////////    Transform public �Լ� /////////////////////////////////////////////////
	const _vec3				Get_Position()												{   return	m_pTransformCom->Get_Pos();								}
	const _vec3				Get_Info(E_INFO eType)										{	return	m_pTransformCom->Get_Info(eType);						}
	const _vec3&			Get_Scale()													{	return	m_pTransformCom->Get_Scale();							}
	const _vec3&			Get_Angle()													{	return	m_pTransformCom->Get_Angle();							}

	virtual void			Set_Pos(_vec3 vPos)											{   m_pTransformCom->Set_Pos(vPos);									}
	virtual void			Set_Pos(E_POSITION ePosition, _float fPos)					{	m_pTransformCom->Set_Pos(ePosition, fPos);						}
	virtual void			Add_Pos(_vec3 vAddPos)										{	m_pTransformCom->Add_Pos(vAddPos);								}
	virtual void			Add_Pos(E_POSITION ePosition, _float fAddPos)				{	m_pTransformCom->Add_Pos(ePosition, fAddPos);					}
	virtual void			Move_Pos(const _vec3& pDir, const _float& fSpeed, const _float& fTimeDelta) { m_pTransformCom->Move_Pos(pDir, fSpeed, fTimeDelta); }

	void					Set_Rotation(_vec3 vRot)									{ m_pTransformCom->Set_Rotation(vRot); }
	void					Set_Rotation(_float fDegX, _float fDegY, _float fDegZ)		{ m_pTransformCom->Set_Rotation(fDegX, fDegY, fDegZ); }
	void					Set_Rotation(E_ROTATION eRotation, _float fDeg)				{ m_pTransformCom->Set_Rotation(eRotation, fDeg); }
	void					Add_Rotation(_vec3 vAddAngle)								{ m_pTransformCom->Add_Rotation(vAddAngle); }
	void					Add_Rotation(E_ROTATION eRotation, _float fDeg)				{	m_pTransformCom->Add_Rotation(eRotation, fDeg);					}

	virtual void					Set_Scale(_vec3 vScale)										{ m_pTransformCom->Set_Scale(vScale); }
	virtual void					Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)	{ m_pTransformCom->Set_Scale(fScaleX, fScaleY, fScaleZ); }
	virtual void					Set_Scale(E_SCALE eScale, _float fScale)					{ m_pTransformCom->Set_Scale(eScale, fScale); }
	void					Add_Scale(_vec3 vAddScale)									{ m_pTransformCom->Add_Scale(vAddScale); }
	void					Add_Scale(E_SCALE eScale, _float fAddScale)					{	m_pTransformCom->Add_Scale(eScale, fAddScale);					}
																								
	void					Set_Look(_vec3 vTargetPos)									{	m_pTransformCom->Set_Look(vTargetPos);							}
	void					Set_Direction(_vec3 vLook)									{   m_pTransformCom->Set_Direction(vLook);							}

	virtual	void			Component_Update()											{ m_pTransformCom->Update_Component(0.f);							}

protected:
	// ���� �ִ� ������, ����:��ȣ
	_matrix					Billboard();

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev = nullptr;
	CTransform*							m_pTransformCom = nullptr;
	map<const wstring, CComponent*>		m_mapComponent;

	E_LAYERTAG		m_eMyLayerTag = E_LAYERTAG::TAG_END;

	_bool			m_bStart		= TRUE;
	_bool			m_bDead			= FALSE;
	_bool			m_bActive		= TRUE;
	E_OBJTAG		m_eObjTag		= E_OBJTAG::OBJ_Player;
	_int			m_iPassNumber	= 0;
	_float			m_fViewZ		= 0.f;
	_float			m_fShaderFilter = 0.f;
	_float			m_fScale	    = 1.f; 
	_vec3			m_vOutTemp		= VEC_ZERO; 


#pragma region Invoke

public:	
	// Invoke �߰��Լ� �����ε�
	void	Invoke_Add(_float fDelayTime, std::function<void(void)> Func);
	void	Invoke_Add(_float fDelayTime, std::function<void(_bool)> Func,	_bool Val);
	void	Invoke_Add(_float fDelayTime, std::function<void(_int)> Func,	_int Val);
	void	Invoke_Add(_float fDelayTime, std::function<void(_float)> Func,	_float Val);
	void	Release_Invoke();

private:
	void	Update_Invoke(const _float& fTimeDelta);

protected:
	// queue<pair<�����̽ð�<pair<���ڿ� ���� ��, �Լ�������>>>>
	queue<pair<_float, std::function<void(void)>>*>						m_qInvoke_void;
	queue<pair<_float, pair<_bool, std::function<void(_bool)>>>*>		m_qInvoke_Bool;
	queue<pair<_float, pair<_float, std::function<void(_float)>>>*>		m_qInvoke_Float;
	queue<pair<_float, pair<_int, std::function<void(_int)>>>*>			m_qInvoke_Int;


#pragma endregion
};

END

#endif // !__GAMEOBJECT_H__
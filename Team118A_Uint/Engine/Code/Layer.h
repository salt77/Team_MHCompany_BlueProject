#pragma once
#ifndef __LAYER_H__
#define __LAYER_H__

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer : public CBase
{
private:
	CLayer();
	virtual ~CLayer();

public:
	static CLayer*		Create();
	virtual void		Free();

public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	_int			LateUpdate_Layer(const _float& fTimeDelta);

#ifdef _DEBUG
	virtual void	Debugging_Console();
#endif

public:
	HRESULT		Add_GameObject(const E_OBJTAG eObjTag, CGameObject* pInstance);
	void		Clear_Layer();
	void		Set_Dead_AllObject();
	void		Create_MapVector(const E_OBJTAG eObjTag);

public:
	CComponent*				Get_Component(const E_OBJTAG eObjTag, _uint iIdx, const wstring& pComponentTag);
	CGameObject*			Get_GameObject(const E_OBJTAG eObjTag, _uint iIdx = 0);
	vector<CGameObject*>*	Get_vecGameObject(const E_OBJTAG eObjTag);
	map<E_OBJTAG, vector<CGameObject*>>* Get_LayerMapObject();	

	const _bool& Get_Active() { return m_bActive; }

public:
	void	Set_LayerTag(E_LAYERTAG eTag) { m_eLayerTag = eTag; }
	void	Set_Active(_bool bState) { m_bActive = bState; }

public:
	_bool	IsExist_Obj(const E_OBJTAG eObjTag); 

private:
	 map<E_OBJTAG , vector<CGameObject*>>		m_mapObject;
	 E_LAYERTAG		m_eLayerTag = E_LAYERTAG::TAG_END;
	 _bool			m_bActive = TRUE;	// 레이어 단위로 끄고 켜기 가능.
};

END

#endif // !__LAYER_H__
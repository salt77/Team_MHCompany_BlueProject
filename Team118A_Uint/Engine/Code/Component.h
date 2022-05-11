#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual CComponent*		Clone() PURE;
	virtual void			Free() override;
	virtual	void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual _int			Update_Component(const _float& fTimeDelta) { return 0; }


	const wstring&	Get_ProtoTag()	const { return m_wstrProtoTag; }
	const wstring&	Get_ComTag()	const { return m_wstrComTag; }



protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bClone = FALSE;
	wstring					m_wstrProtoTag	= L"";
	wstring					m_wstrComTag	= L"";

	


};

END

#endif // !__COMPONENT_H__
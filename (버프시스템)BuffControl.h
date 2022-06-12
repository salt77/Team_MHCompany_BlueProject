#ifndef BuffControl_h__
#define BuffControl_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBuffControl :public CComponent
{
protected:
	explicit CBuffControl(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT& eObjType);
	explicit CBuffControl(const CBuffControl& rhs);
	virtual ~CBuffControl();

public:
	virtual HRESULT			Ready_Component(OBJECT& eObjType);
	virtual _int			Update_Component(const _float& fTimeDelta); 

	void					Compute_Critical(const _matrix* matWorld);
	void					Add_Buff(BUFF_ID eID, _ulong dwBuffDuration);

protected:
	void					Compute_Buff(); 

public:
	virtual void	Free();

private:
	_float						m_fCriticalPotential = 8.f; 
	_float						m_fOriginCritical = 5.f; 

	OBJECT						m_eObjectType = OBJ_END; 
	list<tag_BuffDeBuff*>		m_listBuff;
};


END
#endif // BuffControl_h__

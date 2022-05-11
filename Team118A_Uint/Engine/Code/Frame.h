#ifndef Frame_h__
#define Frame_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame();

public:
	_bool				IsPermit_Call(const _float& fTimeDelta);
	_float				Get_AccTimeDelta();

public:
	HRESULT				Ready_Frame(const _float& fCallLimit);

private:
	_float				m_fCallLimit;			// fps 제한을 하긴 위한 조건변수
	_float				m_fAccTimeDelta = 0.f;		// 누적된 시간 값
	_float				m_fLastAcc = 0.f;

public:
	static CFrame*		Create(const _float& fCallLimit);
	virtual void		Free();

};

END
#endif // Frame_h__

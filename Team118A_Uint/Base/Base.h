#ifndef __Base_H__
#define __Base_H__

/*		변경점 : Explicit 삭제			*/
class _declspec(dllexport) CBase
{
protected:
	inline CBase();
	inline virtual ~CBase();

public:
	inline unsigned long AddRef();
	inline unsigned long Release();

protected:
	unsigned long		m_dwRefCnt;

public:
	inline virtual void Free() = 0;
};

#include "Base.inl"

#endif // __Base_H__

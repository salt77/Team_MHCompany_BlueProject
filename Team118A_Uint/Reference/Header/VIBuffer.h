#pragma once
#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract: public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();


public:			/*		메서드		*/
	virtual void			Free() override;
	virtual	void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

	virtual HRESULT			Ready_Buffer();
	virtual void			Render_Buffer();

	_bool					RayCast(const POINT& vMousePos, 
									HWND hWnd, 
									_uint iWinCX, 
									_uint iWinCY, 
									const _matrix& _matWorld, 
									_vec3* outVec3,
									_bool bOpposite = FALSE);

	const _ulong&			Get_TriCnt();
	const _ulong&			Get_VtxCnt();
	LPDIRECT3DVERTEXBUFFER9 Get_VB();
	LPDIRECT3DINDEXBUFFER9	Get_IB();

protected:		/*		메서드		*/

private:		/*		메서드		*/

protected:		/*		 변수		*/
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxCnt;
	_ulong						m_dwVtxSize;
	_ulong						m_dwTriCnt;
	_ulong						m_dwFVF;

	_ulong						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;

	void*						m_pVertices = nullptr;
	void*						m_pIndices	= nullptr;


private:		/*		 변수		*/

public:			/*		 변수		*/

};

END

#endif // !__VIBUFFER_H__
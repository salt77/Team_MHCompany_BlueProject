#pragma once
#ifndef __RenderTargetMgr_H__
#define __RenderTargetMgr_H__

#include "Engine_Define.h"
#include "Base.h"
#include "RenderTarget.h"

/* �ټ��� ����Ÿ���� �����ϱ����� �Ŵ���Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	CRenderTargetMgr();
	virtual ~CRenderTargetMgr();
	virtual void	Free() override;

public:
	HRESULT					Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
	HRESULT					Ready_MRT(const wstring& pMRTTag, const wstring& pTargetTag);

	HRESULT					Begin_MRT(const wstring& pMRTTag);
	HRESULT					End_MRT(const wstring& pMRTTag);

	void					Clear_ZWrite();

	HRESULT					Ready_DebugBuffer(const wstring& pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void					Render_DebugBuffer(const wstring& pMRTTag);

	void					Set_RenderTargetTexture(LPD3DXEFFECT& pEffect, const wstring& pTargetTag, const char* pConstantTable);

private:
	CRenderTarget*			Find_RenderTarget(const wstring& pTargetTag);
	list<CRenderTarget*>*	Find_MRT(const wstring& pMRTTag);

private:
	/* ������ ����Ÿ���� ��Ƶд�. */
	map<const wstring, CRenderTarget*>			m_mapRenderTarget;

	/* ���ÿ� ��ġ�� ���õǾ��ϴ� Ÿ�ٵ��� ��Ƴ���.  */ 	
	/* ��ǻƮ, ���, ���̵�, */
	map<const wstring, list<CRenderTarget*>>	m_mapMRT;

};

END
#endif // __RenderTargetMgr_H__

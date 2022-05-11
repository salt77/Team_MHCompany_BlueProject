#ifndef __MFC_MAP_H__
#define __MFC_MAP_H__

#include "Base.h"
#include "Engine_Define.h"

#include "GameObject.h"

USING(Engine)

class ENGINE_DLL CStageMap final : public CGameObject
{
private:
	explicit CStageMap(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CStageMap(const CStageMap& rhs);
	virtual ~CStageMap();

public:
	static CStageMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	static CStageMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshTag);
	static CStageMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag);
	virtual void		Free();

public:
	virtual void		Save(HANDLE hFile, DWORD& dwByte);
	virtual void		Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual HRESULT		Ready_Object()									override;
	virtual _int		Update_Object(const _float& fTimeDelta)			override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual void		Render_Object(_int iPass)						override;
	virtual void		Debug_This_In_Console()							override;

public:
	void						Set_BlockRender(_bool pBlockRender)		{ m_bBlockRender = pBlockRender;		}

public:
	virtual const wstring		Get_Name()								{ return m_wstrMeshName;				}
	_bool						Get_BlockRender()						{ return m_bBlockRender;				}

private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	CSMesh*				m_pSMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

private:
	wstring				m_wstrMeshFullPath;
	wstring				m_wstrMeshName;

private:
	_bool				m_bBlockRender = FALSE;
};

#endif // __MFC_MAP_H__

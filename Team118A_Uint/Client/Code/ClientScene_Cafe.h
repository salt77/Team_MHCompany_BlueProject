#pragma once
#ifndef __SCENE_CAFE_H__
#define __SCENE_CAFE_H__

#include "Scene.h"
#include "Invite_Character.h"

class CObstacle_Cafe;
class CClientScene_Cafe final: public CScene
{
private:
	CClientScene_Cafe(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClientScene_Cafe();

public:
	static CClientScene_Cafe*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free() override;

public:
	virtual HRESULT		Ready_Scene() override;
	virtual	_int		Start_Scene(const _float& fTimeDelta) override;
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene() override;

protected:
	HRESULT				Ready_Prototype();
	HRESULT				Ready_LightInfo();
	HRESULT				Ready_SceneObject();

public:
	_vec3				Raycast(HWND hWnd, const _matrix* pMatWorld, _vec3* pOutPos, LPD3DXMESH pMesh, _bool* bCol = nullptr);
	_vec3				TriRaycast(HWND hWnd, CSMesh* pMapBufferCom, CTransform* pMapTransCom, _float* pOutDistance, _bool* bCol);
	_bool				RayChar(_vec3* pOutPos);

public:
	_bool				Get_ColFurniture() { return m_bPickCol; }

public:
	void				Set_UIManagerFunc();
	void				Set_PickFurniture(_bool PickFur) { m_bFurniturePick = PickFur; }
	void				Set_RotFurniture(_bool RotFur) { m_bRotON = RotFur; }
	void				Set_StoFurniture(_bool StoFur) { m_bStorageON = StoFur; }
	void				Set_AllStoFurniture(_bool StoFur) { m_bAllStorageON = StoFur; }
	void				Set_SelectFurInfo(_bool SelFurInfo) { m_bSelectFurInfo = SelFurInfo; }
	void				Set_CreateFurPick(_bool SelFurInfo) { m_bCreateFurPick = SelFurInfo; }
	void				Set_OpenAmenity(_bool Amenity) { m_bOpenAmenity = Amenity; }
	void				Set_OpenFurInfo(_bool OpenFurniture) { m_bOpenFurInfo = OpenFurniture; }
	void				Set_ExitFurInfo(_bool ExitFurniture) { m_bExitFurInfo = ExitFurniture; }
	void				Set_UpdateAmenity(_bool Amenity) { m_bStartAmenity = Amenity; }
	void				Set_Recieve(_bool Profit) { m_bRecieve = Profit; }
	void				Set_PresetName(_bool Preset) { m_bPresetName = Preset; }
	void				Set_PresetNameEnd(_bool Preset) { m_bPresetNameEnd = Preset; }
	void				Set_PresetName1(_bool Preset) { m_bPresetName1 = Preset; }
	void				Set_PresetName2(_bool Preset) { m_bPresetName2 = Preset; }
	void				Set_PresetName3(_bool Preset) { m_bPresetName3 = Preset; }
	void				Set_PresetName4(_bool Preset) { m_bPresetName4 = Preset; }
	void				Set_PresetName5(_bool Preset) { m_bPresetName5 = Preset; }

public:
	void				Set_PresetInit1(_bool Preset1) { m_bPresetInit1 = Preset1; }
	void				Set_PresetSave1(_bool Preset1) { m_bPresetSave1 = Preset1; }
	void				Set_PresetApply1(_bool Preset1) { m_bPresetApply1 = Preset1; }
	void				Set_PresetInit2(_bool Preset2) { m_bPresetInit2 = Preset2; }
	void				Set_PresetSave2(_bool Preset2) { m_bPresetSave2 = Preset2; }
	void				Set_PresetApply2(_bool Preset2) { m_bPresetApply2 = Preset2; }
	void				Set_PresetInit3(_bool Preset3) { m_bPresetInit3 = Preset3; }
	void				Set_PresetSave3(_bool Preset3) { m_bPresetSave3 = Preset3; }
	void				Set_PresetApply3(_bool Preset3) { m_bPresetApply3 = Preset3; }
	void				Set_PresetInit4(_bool Preset4) { m_bPresetInit4 = Preset4; }
	void				Set_PresetSave4(_bool Preset4) { m_bPresetSave4 = Preset4; }
	void				Set_PresetApply4(_bool Preset4) { m_bPresetApply4 = Preset4; }
	void				Set_PresetInit5(_bool Preset5) { m_bPresetInit5 = Preset5; }
	void				Set_PresetSave5(_bool Preset5) { m_bPresetSave5 = Preset5; }
	void				Set_PresetApply5(_bool Preset5) { m_bPresetApply5 = Preset5; }

public:
	void				Set_Gift1(_bool Gift1) { m_bGift1 = Gift1; }
	void				Set_Gift2(_bool Gift2) { m_bGift2 = Gift2; }
	void				Set_Gift3(_bool Gift3) { m_bGift3 = Gift3; }
	void				Set_Gift4(_bool Gift4) { m_bGift4 = Gift4; }
	void				Set_Gift5(_bool Gift5) { m_bGift5 = Gift5; }

public:
	virtual void		Set_FogEvent();
	virtual void		Update_FogEvent(const _float& fTimeDelta);

private:
	void				LoadCafeMap(const wstring & wstrFilePath);
	void				LoadCafeFurniture(const wstring & wstrFilePath);
	void				CreateBlock();
	void				SelectFurniture();
	void				PickingFurniture();
	void				MoveFurniture();
	void				RotateFurniture();
	void				DeleteFurniture();

private:
	void				PickChar();
	void				MoveChar();
	void				CharSound();

private:
	CUI_Object*			Load_UI_Prefab(wstring wstrPath);

	void				UI_SelFurInfo();
	void				UI_CreateFurPick();
	void				UI_StartAmenityInfo();
	void				UI_AmenityInfo();
	void				UI_FurnitureInfo();
	void				UI_DeleteFurnitureInfo();
	void				UI_UpdateProfit(const _float& fTimeDelta);
	void				UI_Preset(const _float& fTimeDelta);
	void				UI_PresetSave(_int iIndex);
	void				UI_PresetApply(_int iIndex, const _float& fTimeDelta);
	void				UI_PresetName();
	void				UI_InputKey();

private:
	CStageMap*			m_pMap = nullptr;
	CStageMap*			m_pTile = nullptr;
	CObstacle_Cafe*		m_pObstacle_Cafe = nullptr;
	CGameObject*		m_pSelFurniture = nullptr;
	CGameObject*		m_pSelFurnitureW = nullptr;
	CStageMap*			m_pBlock = nullptr;
	CStageMap*			m_pBlock_L = nullptr;
	CStageMap*			m_pBlock_R = nullptr;
	vector<CStageMap*>	m_vecBlock;
	vector<CStageMap*>	m_vecBlock_L;
	vector<CStageMap*>	m_vecBlock_R;
	vector<CGameObject*>* m_vecObstacle_Cafe;
	CUI_Object*			m_pUi_Furniture;
	vector<CUI_Object*> m_vecUi_Furniture;

private:
	_bool				m_bFurniturePick = false;
	_bool				m_bRotON = false;
	_float				m_fRot = 0.f;
	_bool				m_bStorageON = false;
	_bool				m_bAllStorageON = false;
	_bool				m_bPickCol = false;
	_bool				m_bTileCol = false;
	_bool				m_bBlockLCol = false;
	_bool				m_bBlockRCol = false;
	_bool				m_bCreate = true;
	_bool				m_bOnefor = true;
	_bool				m_bSelectFurInfo = false;
	_bool				m_bCreateFurPick = false;
	_int				m_iAmenity = 0;
	_bool				m_bStartAmenity = true;
	_bool				m_bOpenAmenity = false;
	_bool				m_bLoadPrefab = true;
	_bool				m_bOpenFurInfo = false;
	_bool				m_bExitFurInfo = false;
	_float				m_fProfitTime = 0.f;
	_int				m_iProfitIndex = 0;
	_int				m_iCreditIndex = 0;
	_int				m_iAPIndex = 0;
	_bool				m_bRecieve = false;
	_bool				m_bPresetName = false;
	_bool				m_bPresetNameEnd = false;
	_bool				m_bPresetName1 = false;
	_bool				m_bPresetName2 = false;
	_bool				m_bPresetName3 = false;
	_bool				m_bPresetName4 = false;
	_bool				m_bPresetName5 = false;

private:
	_bool				m_bPresetInit1 = false;
	_bool				m_bPresetSave1 = false;
	_bool				m_bPresetApply1 = false;
	_bool				m_bPresetInit2 = false;
	_bool				m_bPresetSave2 = false;
	_bool				m_bPresetApply2 = false;
	_bool				m_bPresetInit3 = false;
	_bool				m_bPresetSave3 = false;
	_bool				m_bPresetApply3 = false;
	_bool				m_bPresetInit4 = false;
	_bool				m_bPresetSave4 = false;
	_bool				m_bPresetApply4 = false;
	_bool				m_bPresetInit5 = false;
	_bool				m_bPresetSave5 = false;
	_bool				m_bPresetApply5 = false;
	_bool				m_bPresetLock1 = false;
	_bool				m_bPresetLock2 = false;
	_bool				m_bPresetLock3 = false;
	_bool				m_bPresetLock4 = false;
	_bool				m_bPresetLock5 = false;

private:
	_bool				m_bGift1 = false;
	_bool				m_bGift2 = false;
	_bool				m_bGift3 = false;
	_bool				m_bGift4 = false;
	_bool				m_bGift5 = false;

private:
	CInvite_Character*	m_pInvite_Char = nullptr;
	vector<CInvite_Character*>	m_vecInvite_Char;
	CInvite_Character*	m_pSelChar = nullptr;
	_bool				m_bCharPick = false;
	CGameObject*		m_pCamera = nullptr;

private:
	static _bool		bUniquePrototype;

private:
	_bool				m_bPresetFog = false;
	_float				m_fPresetFogTime = 0.f;
};

#endif // !__SCENE_CAFE_H__
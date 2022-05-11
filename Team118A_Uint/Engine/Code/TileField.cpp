#include "TileField.h"
#include "UI_Object.h"
#include "UI_Manager.h"
#include "Renderer.h"
#include "GameManager.h"
#include "XL_ParsingMgr.h"

//몬스터 목록

//특별의뢰 몬스터

USING(Engine)

//저장된 월드를 불러오는 생성자 
CTileField::CTileField()
{
	ZeroMemory(&m_arrayCombatTile, sizeof(m_arrayCombatTile));
}

CTileField::~CTileField()
{
}

CTileField * CTileField::Create(wstring wstrTileFieldName)
{
	CTileField* pInstance = new CTileField();



	if (FAILED(pInstance->Ready_TileField(wstrTileFieldName)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTileField::Free()
{
	for (auto& pMascot : m_vecPlayerMascot)
	{
		Safe_Release(pMascot);
	}

	for (auto& pMascot : m_vecMonsterMascot)
	{
		Safe_Release(pMascot);
	}

	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		Safe_Release(m_arrayCombatTile[i]);
	}

	for (auto& tPair : m_mapStartWorldUI)
	{
		Safe_Release(tPair.second);
	}
	m_mapStartWorldUI.clear();

	for (auto& pStaticObj : m_vecEnableMesh)
	{
		Safe_Release(pStaticObj);
	}
	m_vecEnableMesh.clear();

	for (auto& pStaticObj : m_vecSelectTile)
	{
		Safe_Release(pStaticObj);
	}

	Safe_Release(m_pCheckUI);
	Safe_Release(m_pTileFieldUI);

	Safe_Release(m_pPickRectBuffer);

	Safe_Release(m_pMyPhase);
	Safe_Release(m_pEnemyPhase);
	Safe_Release(m_pStageStart);

	Safe_Release(m_pCharReadyUI);
	Safe_Release(m_pCharCombatUI);
}

void CTileField::Update_TileField(const _float & fTimeDelta)
{
	Clear_DeadMascot();
	if (CUI_Manager::GetInstance()->m_pCombatVictoryPanel)
	{
		CUI_Manager::GetInstance()->m_pCombatVictoryPanel->Set_Active(FALSE);
	}

	if (m_bCheck)
	{
		Check();
		return;
	}

	if (m_bGameStart)
	{
		if (m_bPlayerTurn)
		{
			Player_Update(fTimeDelta);
		}
		else
		{
			Monster_Update(fTimeDelta);
		}

		
	}
	else
	{
		if (!m_pCharReadyUI->Get_Active())
		{
			if (m_bUIActive)
			{
				if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !m_bKeyDown)
				{
					Pickup_Mascot();
					if (!m_pPickMascot)
					{
						Pick_StartPos();
					}
					m_bKeyDown = TRUE;
				}
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					Pick_Move();
				}
				if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000) && m_bKeyDown)
				{
					Pick_End();

					m_bKeyDown = FALSE;
				}
			}
		}
	}


}

HRESULT CTileField::Ready_TileField(wstring wstrTileFieldName)
{
	m_wstrTileFieldName = wstrTileFieldName;

	HRESULT hResult = Load_TileField(wstrTileFieldName);

	for (_int i = 0; i < (_int)m_mapStartWorldUI.size(); ++i)
	{
		m_vecSelectTile.emplace_back(CStaticObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Test, L"../../Reference/Resource/TileMesh/CombatTile_Player.X"));
	}
	

	m_vecEnableMesh.reserve(6);
	for (_int i = 0; i < 6; ++i)
	{
		m_vecEnableMesh.emplace_back(CStaticObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Test, L"../../Reference/Resource/TileMesh/CombatTileSelect.X"));
	}

	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		Set_NearTileIndex(i);
	}

	for (auto& pSelectTile : m_vecSelectTile)
	{
		pSelectTile->Set_Active(FALSE);
	}

	for (auto& pEnableTile : m_vecEnableMesh)
	{
		pEnableTile->Set_Active(FALSE);
	}

	_int iEnableMeshIndex = 0;
	for (auto& tPair : m_mapStartWorldUI)
	{
		m_vecEnableMesh[iEnableMeshIndex]->Set_Pos(m_arrayCombatTile[tPair.first]->Get_Position());
		m_vecEnableMesh[iEnableMeshIndex]->Set_Active(TRUE);
		++iEnableMeshIndex;
	}
	
	m_pPickRectBuffer = CRectBuffer::Create(CGraphicDev::GetInstance()->GetDevice());

	m_pCheckUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]알림UI.uidat");
	if (!m_pCheckUI)
	{
		return E_FAIL;
	}
	m_pCheckUI->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Check_Cancle, this));
	m_pCheckUI->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Check_Accept, this));
	m_pCheckUI->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Check_Cancle, this));
	m_pCheckUI->Set_Active(FALSE);
	
	m_pTileFieldUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Tile_UI.uidat");
	if (!m_pTileFieldUI)
	{
		return E_FAIL;
	}

	m_pTileFieldUI->Get_ChildByIndex(1)->Set_Active(FALSE);
	m_pTileFieldUI->Get_ChildByIndex(7)->Set_Active(FALSE);
	m_pTileFieldUI->Get_ChildByIndex(7)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Change_Squad, this));
	m_pTileFieldUI->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Game_Start, this));
	m_pTileFieldUI->Get_ChildByIndex(8)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Change_TurnButton, this));
	m_pTileFieldUI->Get_ChildByIndex(9)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Change_AutoTurnChange, this));
	m_pTileFieldUI->Get_ChildByIndex(9)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::Change_AutoTurnChange, this));
	
	WCHAR wszReadySquad[8] = L"";
	swprintf_s(wszReadySquad, L"%d/%d", 0, (_int)m_mapStartWorldUI.size());
	m_pTileFieldUI->Get_ChildByIndex(4)->Set_Ui_Text(wszReadySquad);
	Set_EnemyInfo();

	m_pMyPhase = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]MyPhase.uidat");
	m_pEnemyPhase = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]EnemyPhase.uidat");
	m_pStageStart = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]StageStart.uidat");

	m_pCharReadyUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]CharacterReadyUI.uidat");
	m_pCharReadyUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharReady_Release, this));
	m_pCharReadyUI->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharReady_Swap, this));
	m_pCharReadyUI->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharReady_Trans, this));
	m_pCharReadyUI->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharReady_Cancel, this));

	m_pCharCombatUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]CharacterCombatUI.uidat");
	m_pCharCombatUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharCombat_Select, this));
	m_pCharCombatUI->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharCombat_Trans, this));
	m_pCharCombatUI->Add_Function_void(E_UiFunctionType::OnClick, bind(&CTileField::CharCombat_Cancel, this));

	return hResult;
}

void CTileField::Spawn_FieldTile()
{
	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		if (E_TILE_OPTION::TILE_EMPTY == m_arrayCombatTile[i]->Get_TileOption())
		{
			continue;
		}

		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Test, m_arrayCombatTile[i]);
		m_arrayCombatTile[i]->AddRef();
	}

	for (CMascot* pMascot : m_vecMonsterMascot)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, pMascot->Get_ObjTag(), pMascot);
		pMascot->AddRef();
	}

	for (auto& tPair : m_mapStartWorldUI)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, tPair.second->Get_ObjTag(), tPair.second);
		tPair.second->AddRef();
	}

	for (auto& pStaticObj : m_vecSelectTile)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, pStaticObj->Get_ObjTag(), pStaticObj);
		pStaticObj->AddRef();
	}

	for (auto& pEnableTile : m_vecEnableMesh)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, pEnableTile->Get_ObjTag(), pEnableTile);
		pEnableTile->AddRef();
	}

	for (auto& pPlayerMascot : m_vecPlayerMascot)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Character, pPlayerMascot->Get_ObjTag(), pPlayerMascot);
		pPlayerMascot->AddRef();
	}


	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pTileFieldUI);
	m_pTileFieldUI->AddRef();

	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pCheckUI);
	m_pCheckUI->AddRef();

	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pMyPhase);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pEnemyPhase);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pStageStart);
	m_pMyPhase->AddRef();
	m_pEnemyPhase->AddRef();
	m_pStageStart->AddRef();
	m_pMyPhase->Set_Active(FALSE);
	m_pEnemyPhase->Set_Active(FALSE);
	m_pStageStart->Set_Active(FALSE);


	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pCharCombatUI);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pCharReadyUI);
	m_pCharCombatUI->Set_Active(FALSE);
	m_pCharReadyUI->Set_Active(FALSE);
	m_pCharCombatUI->AddRef();
	m_pCharReadyUI->AddRef();

	
	
}

void CTileField::Game_Start()
{
	CGameManager::GetInstance()->Set_IsTileFiledBattle(TRUE);
	if (m_bGameStart)
	{
		return;
	}

	if (m_vecPlayerMascot.empty())
	{
		return;
	}

	if (m_vecPlayerMascot.size() < m_mapStartWorldUI.size())
	{
		//알림
		m_eCheck = E_CHECK::CHECK_START;
		Open_CheckUI(m_eCheck);
		return;
	}

	Start_Game();
}

void CTileField::Start_Game()
{
	m_bGameStart = TRUE;

	for (auto& tPair : m_mapStartWorldUI)
	{
		tPair.second->Get_UIObj()->Get_ChildByIndex(0)->Set_Active(TRUE);
		tPair.second->Get_UIObj()->Get_ChildByIndex(1)->Set_Active(FALSE);
	}

	if (m_pCharReadyUI->Get_Active())
	{
		CharReady_Cancel();
	}

	m_pTileFieldUI->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pTileFieldUI->Get_ChildByIndex(4)->Set_Active(FALSE);

	m_pTileFieldUI->Get_ChildByIndex(8)->Set_Active(TRUE);
	m_pTileFieldUI->Get_ChildByIndex(9)->Set_Active(TRUE);

	for (auto& pEnableTile : m_vecEnableMesh)
	{
		pEnableTile->Set_Active(FALSE);
	}

	m_bCheck = TRUE;
	m_fUITimer = CScene::Get_PlayTime();
	m_pStageStart->Set_Active(TRUE);
	m_eCheck = CHECK_STAGE_START;
	m_bKeyDown = FALSE;

	CUI_Manager::GetInstance()->BattleFormation_ParticpateReset();
}

void CTileField::Add_Squad_Mascot(CMascot * pMascot)
{
	for (auto& pPlayer : m_vecPlayerMascot)
	{
		if (pPlayer->Get_CurrentTileIndex() == m_iSelectIndex)
		{
			Release_PlayerMascot(pPlayer);
			break;
		}
	}

	m_vecPlayerMascot.emplace_back(pMascot);
	pMascot->Set_Rotation(_vec3(0.f, 150.f, 0.f));
	pMascot->Set_Pos(m_arrayCombatTile[m_iSelectIndex]->Get_Position());
	pMascot->Set_CurrentTileIndex(m_iSelectIndex);
	pMascot->Set_StartIndex(m_iSelectIndex);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Character, pMascot->Get_ObjTag(), pMascot);
	pMascot->AddRef();

	WCHAR wszReadySquad[8] = L"";
	swprintf_s(wszReadySquad, L"%d/%d", (_int)m_vecPlayerMascot.size(), (_int)m_mapStartWorldUI.size());
	m_pTileFieldUI->Get_ChildByIndex(4)->Set_Ui_Text(wszReadySquad);


	Set_SquadWindow(pMascot);

	SetUIActive(TRUE);

}

vector<E_SQUAD> CTileField::Get_Squad()
{
	vector<E_SQUAD> vecSquad;

	for (auto& pMascot : m_vecPlayerMascot)
	{
		vecSquad.emplace_back(pMascot->Get_Squad());
	}

	return vecSquad;
}

void CTileField::Change_Turn()
{
	if (!m_bGameStart)
	{
		return;
	}

	if (!m_bPlayerTurn)
	{
		Check_WeakTile();
		++m_iTurn;
		m_bPlayerTurn = TRUE;
		
		for (auto& pMascot : m_vecPlayerMascot)
		{
			pMascot->Set_AP(1);
			pMascot->Set_WarpPoint(1);
		}

		m_vecSelectTile.front()->Set_Active(TRUE);
		Select_NextPlayer();
	}
	else
	{
		m_bPlayerTurn = FALSE;

		for (auto& pMascot : m_vecMonsterMascot)
		{
			pMascot->Set_AP(1);
		}
		m_vecSelectTile.front()->Set_Active(FALSE);

		for (auto& pEnableMesh : m_vecEnableMesh)
		{
			pEnableMesh->Set_Active(FALSE);
		}

		
	}
}

void CTileField::Call_TurnChange()
{
	m_fUITimer = CScene::Get_PlayTime();
	m_bCheck = TRUE;

	WCHAR wszTurnName[8] = L"";
	


	if (m_bPlayerTurn)
	{
		swprintf_s(wszTurnName, L"Turn %d", m_iTurn);
		m_pEnemyPhase->Get_ChildByIndex(1)->Set_Ui_Text(wszTurnName);
		m_pEnemyPhase->Set_Active(TRUE);
		
	}
	else
	{
		swprintf_s(wszTurnName, L"Turn %d", m_iTurn+1);
		m_pMyPhase->Get_ChildByIndex(1)->Set_Ui_Text(wszTurnName);
		m_pMyPhase->Set_Active(TRUE);
	}

	m_eCheck = E_CHECK::CHECK_TURN_END;
}


void CTileField::End_War(_bool bWin)
{
	if (!m_pWarPlayer || !m_pWarMonster)
	{
		return;
	}

	if (bWin)
	{
		m_pWarMonster->Set_MascotDead(TRUE);

		if (m_pWarMonster->Get_Boss())
		{
			CGameManager::GetInstance()->Set_IsTileFiledBattle(FALSE);
		}
	}
	else
	{
		m_pWarPlayer->Set_MascotDead(TRUE);
		_bool bCheck = TRUE;
		for (auto& pPlayer : m_vecPlayerMascot)
		{
			if (!pPlayer->Get_MascotDead())
			{
				bCheck = FALSE;
				break;
			}
		}

		if(bCheck)
		{
			CGameManager::GetInstance()->Set_IsTileFiledBattle(FALSE);
		}

	}
	m_pWarPlayer = nullptr;
	m_pWarMonster = nullptr;

	if (m_bPlayerTurn)
	{
		Select_NextPlayer();
	}
	else
	{
		
	}

	m_bCombatSceneChange = FALSE;
}

void CTileField::Close_CheckUI()
{
	m_pCheckUI->Set_Active(FALSE);
}

HRESULT CTileField::Load_TileField(wstring wstrTileFieldName)
{
	wstring wstrLocalPath = L"../../Reference/Data/Data_Tile/" + wstrTileFieldName;

	HANDLE hFile = CreateFile(wstrLocalPath.c_str(), GENERIC_READ, NULL, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	DWORD dwByte;


	//타일 불러오기
	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		_int iOption;
		CUtility::Data_ReadInt(hFile, iOption, dwByte);
		m_arrayCombatTile[i] = CCombatTile::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Test, E_TILE_OPTION(iOption));
		_vec3 vPos;
		CUtility::Data_ReadVec3(hFile, vPos, dwByte);
		m_arrayCombatTile[i]->Set_Pos(vPos);
		_int iLink;
		CUtility::Data_ReadInt(hFile, iLink, dwByte);
		m_arrayCombatTile[i]->Set_LinkTileIndex(iLink);

		if (iOption == (_int)E_TILE_OPTION::TILE_START)
		{
			CWorldUI* pWorldUI = CWorldUI::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Test, L"StartUI.uidat");

			assert(pWorldUI);

			pWorldUI->Set_Pos(vPos);
			m_mapStartWorldUI.emplace(i,pWorldUI);
		}
	}

	_int iSize;
	CUtility::Data_ReadInt(hFile, iSize, dwByte);

	//적 불러오기
	CMascot* pMascot = nullptr;
	for (_int i = 0; i<iSize; ++i)
	{
		wstring wstrWorldName;
		CUtility::Data_ReadWString(hFile, wstrWorldName, dwByte);
		_int iCurrentIndex;
		CUtility::Data_ReadInt(hFile, iCurrentIndex, dwByte);
		_int iPlayType;
		CUtility::Data_ReadInt(hFile, iPlayType, dwByte);

		pMascot = CMascot::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster, wstrWorldName);
		if (!pMascot)
		{
			CloseHandle(hFile);
			return E_FAIL;
		}

		pMascot->Set_CurrentTileIndex(iCurrentIndex);
	
		_vec3 vPos;
		CUtility::Data_ReadVec3(hFile, vPos, dwByte);
		pMascot->Set_Pos(vPos);
		_vec3 vRot;
		CUtility::Data_ReadVec3(hFile, vRot, dwByte);
		pMascot->Set_Rotation(vRot);

		pMascot->Set_PlayType(E_ENEMY_PLAY_TYPE(iPlayType));

		m_vecMonsterMascot.emplace_back(pMascot);
		Add_MonsterPool(pMascot->Get_MonsterPoolInfo());
		
	}

	CloseHandle(hFile);

	return S_OK;
}

void CTileField::Add_MonsterPool(const vector<T_MONSTER_INFO>& vecMonsterPool)
{

	for (auto& tMonsterInfo : vecMonsterPool)
	{
		_bool bCheck = TRUE;
		for (auto& tTileFieldMonsterInfo : m_vecMonsterPool)
		{
			if (!lstrcmp(tMonsterInfo.wszDisplayName, tTileFieldMonsterInfo.wszDisplayName))
			{
				bCheck = FALSE;
				break;
			}
		}
		if (bCheck)
		{
			m_vecMonsterPool.emplace_back(tMonsterInfo);
		}
	}
}

void CTileField::Player_Update(_float fTimeDelta)
{
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !m_bKeyDown)
	{
		m_bKeyDown = TRUE;
	}
	else if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000) && m_bKeyDown)
	{
		if (m_pSelectMascot)
		{
			if (m_pSelectMascot->Get_AP() > 0)
			{
				if (Pick_SwapTile())
				{
					return;
				}
			}
			if (m_pSelectMascot->Get_AP() > 0)
			{
				Pick_MoveTile();
			}
			if (m_pSelectMascot->Get_WarpPoint() > 0)
			{
				Pick_WarpTile();
			}
		}
		m_bKeyDown = FALSE;
	}
			
}

void CTileField::Monster_Update(_float fTimeDelta)
{
	_bool bAction = FALSE;
	for (auto& pMonster : m_vecMonsterMascot)
	{
		if (pMonster->Get_MascotDead())
		{
			continue;
		}
		if (pMonster->Get_AP() < 1)
		{
			continue;
		}
		if (pMonster->Get_PlayType() == E_ENEMY_PLAY_TYPE::TYPE_DEFENSIVE)
		{
			continue;
		}
		m_pSelectMascot = pMonster;
		if (Action_Monster())
		{
			bAction = TRUE;
			break;
		}
	}
	
	if (!bAction)
	{
		Call_TurnChange();
		
	}

}

void CTileField::Pickup_Mascot()
{
	for (auto& pPlayer : m_vecPlayerMascot)
	{
		CSphereCollider* pSphereCol = static_cast<CSphereCollider*>(pPlayer->Get_Component(ComTag_Collider));
		CGameObject* pColMascot = nullptr;
		if (pSphereCol->PickingCollider(*CUI_Manager::GetInstance()->Get_Hwnd(), &pColMascot))
		{
			m_pPickMascot = pPlayer;
			POINT pt = CUtility::Get_MousePoint(*CUI_Manager::GetInstance()->Get_Hwnd());
			
			m_fPickTimer = CScene::Get_PlayTime();
			
		}
	}
}

void CTileField::Pick_Move()
{
	if (!m_pPickMascot)
	{
		return;
	}

	if (m_fPickTimer + 0.3f < CScene::Get_PlayTime())
	{
		POINT pt = CUtility::Get_MousePoint(*CUI_Manager::GetInstance()->Get_Hwnd());

		_vec3 vPos;
		_float fDist;

		_matrix matScale, matRotX;
		D3DXMatrixScaling(&matScale, 100000.f, 100000.f, 100000.f);
		D3DXMatrixRotationX(&matRotX, 90.f * Deg2Rad);

		if (m_pPickRectBuffer->Picking_Buffer(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDist, matScale*matRotX))
		{
			m_pPickMascot->Set_Pos(vPos);
			m_pPickMascot->Add_Pos(_vec3(0.f, 2.f, 0.f));
			m_pPickMascot->Set_ActionByEnum(E_ACTION::Formation_Pickup);
		}
	}



}

void CTileField::Pick_End()
{
	if (!m_pPickMascot)
	{
		return;
	}

	if (m_fPickTimer + 0.3f < CScene::Get_PlayTime())
	{
		m_pPickMascot->Set_Scale(2.5f, 2.5f, 2.5f);
		m_pPickMascot->Set_ActionByEnum(E_ACTION::Normal_Idle);

		for (auto& tPair : m_mapStartWorldUI)
		{
			_int iStartIndex = tPair.first;

			if (iStartIndex == m_pPickMascot->Get_StartIndex())
			{
				continue;
			}

			_matrix matWorld = tPair.second->Get_Transform()->Get_WorldMatrix();
			CSMesh* pSMesh = static_cast<CSMesh*>(m_arrayCombatTile[iStartIndex]->Get_Component(ComTag_SMesh));

			if (!pSMesh)
			{
				return;
			}

			_vec3 vPos;
			_float fDist;
			if (pSMesh->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDist, matWorld))
			{
				for (auto& pPlayer : m_vecPlayerMascot)
				{
					if (pPlayer == m_pPickMascot)
					{
						continue;
					}

					if (iStartIndex == pPlayer->Get_CurrentTileIndex())
					{
						pPlayer->Set_Pos(m_arrayCombatTile[m_pPickMascot->Get_CurrentTileIndex()]->Get_Position());
						pPlayer->Set_StartIndex(m_pPickMascot->Get_StartIndex());
						pPlayer->Set_CurrentTileIndex(m_pPickMascot->Get_StartIndex());
					}
				}

				m_pPickMascot->Set_Pos(m_arrayCombatTile[iStartIndex]->Get_Position());
				m_pPickMascot->Set_StartIndex(iStartIndex);
				m_pPickMascot->Set_CurrentTileIndex(iStartIndex);

				m_pPickMascot = nullptr;
				return;
			}
		}

		m_pPickMascot->Set_Pos(m_arrayCombatTile[m_pPickMascot->Get_StartIndex()]->Get_Position());
		m_pPickMascot = nullptr;
	}
	else
	{
		_vec2 vUIPos = CUtility::World2Ui(m_pPickMascot->Get_Position());

		m_pCharReadyUI->Set_Ui_PosX((_int)vUIPos.x);
		m_pCharReadyUI->Set_Ui_PosY((_int)vUIPos.y);

		m_pCharReadyUI->Set_Active(TRUE);
	
		m_eCheck = E_CHECK::CHECK_CHAR_READY;
		m_iAnswer = -1;
		m_bCheck = TRUE;
	}
}

void CTileField::Pick_StartPos()
{

	for (auto& tPair : m_mapStartWorldUI)
	{
		_int iStartTileIndex = tPair.first;
		_vec3 vPos;
		_float fDis;
		_matrix matWorld = static_cast<CTransform*>(m_arrayCombatTile[iStartTileIndex]->Get_Component(ComTag_Transform))->Get_WorldMatrix();

		if (TRUE == static_cast<CSMesh*>(m_arrayCombatTile[iStartTileIndex]->Get_Component(ComTag_SMesh))->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDis, matWorld))
		{
			for (auto& pMascot : m_vecPlayerMascot)
			{
				if (iStartTileIndex == pMascot->Get_CurrentTileIndex())
				{
					continue;
				}
			}

			m_iSelectIndex = iStartTileIndex;
			SetUIActive(FALSE);
			CUI_Manager::GetInstance()->Open_Ui_BattleFormation((_int)E_WORLD_MOD::MOD_MISSION);
			return;
		}
	}
}

void CTileField::Select_Player(_int iIndex)
{
	if (iIndex < 0 || iIndex >= (_int)m_vecPlayerMascot.size())
	{
		return;
	}

	m_vecSelectTile.front()->Set_Pos(m_vecPlayerMascot[iIndex]->Get_Position());
	m_pSelectMascot = m_vecPlayerMascot[iIndex];

	m_vecSelectTile.front()->Set_Active(TRUE);

	if (m_pSelectMascot->Get_AP() > 0)
	{
		Enable_Check(m_pSelectMascot->Get_CurrentTileIndex());
	}

	Set_SquadWindow(m_vecPlayerMascot[iIndex]);
}

void CTileField::Select_NextPlayer()
{
	_int iIndex = 0;
	for (auto& pMascot : m_vecPlayerMascot)
	{
		if (pMascot->Get_MascotDead())
		{
			continue;
		}

		if (pMascot->Get_AP() > 0)
		{
			Select_Player(iIndex);
			return;
		}
		++iIndex;
	}

	if (m_bAutoTurnChange)
	{
		Call_TurnChange();
	}

}



void CTileField::Enable_Check(_int iTileIndex)
{
	if (iTileIndex < 0 || iTileIndex >= m_iMaxArray)
	{
		return;
	}

	for (auto& pEnableMesh : m_vecEnableMesh)
	{
		pEnableMesh->Set_Active(FALSE);
	}

	_int iIndex = 0;

	if (m_pSelectMascot->Get_AP() > 0)
	{
		for (auto& iNearTileIndex : m_arrayCombatTile[iTileIndex]->Get_NearTileIndex())
		{
			if (!m_arrayCombatTile[iNearTileIndex]->Get_Active())
			{
				continue;
			}
		
			m_vecEnableMesh[iIndex]->Set_Pos(m_arrayCombatTile[iNearTileIndex]->Get_Position());
			m_vecEnableMesh[iIndex]->Set_Active(TRUE);
			++iIndex;
		}
	}

	
}

void CTileField::Pick_MoveTile()
{
	if (!m_pSelectMascot)
	{
		return;
	}
	
	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Player)
	{
		return;
	}

	for (auto& iEnableIndex : m_arrayCombatTile[m_pSelectMascot->Get_CurrentTileIndex()]->Get_NearTileIndex())
	{
		if (!m_arrayCombatTile[iEnableIndex]->Get_Active())
		{
			continue;
		}


		_vec3 vPos;
		_float fDis;
		_matrix matWorld = static_cast<CTransform*>(m_arrayCombatTile[iEnableIndex]->Get_Component(ComTag_Transform))->Get_WorldMatrix();

		if (TRUE == static_cast<CSMesh*>(m_arrayCombatTile[iEnableIndex]->Get_Component(ComTag_SMesh))->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDis, matWorld))
		{
			vPos = m_arrayCombatTile[iEnableIndex]->Get_Position();
			m_pSelectMascot->Set_MovePos(vPos);
			m_bCheck = TRUE;
			m_eCheck = E_CHECK::CHECK_MOVE;
			m_iMoveIndex = iEnableIndex;
			
			m_pSelectMascot->Set_AP(0);

			for (auto& pEnableMesh : m_vecEnableMesh)
			{
				pEnableMesh->Set_Active(FALSE);
			}

			m_vecSelectTile.front()->Set_Active(FALSE);

			m_vecEnableMesh.front()->Set_Active(TRUE);
			m_vecEnableMesh.front()->Set_Pos(vPos);
			return;
		}

	}
}

void CTileField::Pick_WarpTile()
{
	if (!m_pSelectMascot)
	{
		return;
	}

	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Player)
	{
		return;
	}

	_int iIndex = m_pSelectMascot->Get_CurrentTileIndex();

	if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_WARP_ONE_WAY_START || m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_WARP_TWO_WAY))
	{
		return;
	}

	_vec3 vPos;
	_float fDis;
	_matrix matWorld = static_cast<CTransform*>(m_arrayCombatTile[iIndex]->Get_Component(ComTag_Transform))->Get_WorldMatrix();

	if (TRUE == static_cast<CSMesh*>(m_arrayCombatTile[iIndex]->Get_Component(ComTag_SMesh))->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDis, matWorld))
	{
		m_eCheck = E_CHECK::CHECK_WARP;
		Open_CheckUI(m_eCheck);

		return;
	}
}

_bool CTileField::Pick_SwapTile()
{
	if (!m_pSelectMascot)
	{
		return FALSE;
	}

	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Player)
	{
		return FALSE;
	}

	for (auto& iEnableIndex : m_arrayCombatTile[m_pSelectMascot->Get_CurrentTileIndex()]->Get_NearTileIndex())
	{
		CMascot* pSwapMascot = nullptr;

		for (auto& pPlayer : m_vecPlayerMascot)
		{
			if (pPlayer->Get_CurrentTileIndex() == m_pSelectMascot->Get_CurrentTileIndex())
			{
				continue;
			}

			if (iEnableIndex == pPlayer->Get_CurrentTileIndex())
			{
				pSwapMascot = pPlayer;
				break;
			}
		}
		if (!pSwapMascot)
		{
			continue;
		}


		_vec3 vPos;
		_vec3 vPos2;
		_float fDis;
		_matrix matWorld = static_cast<CTransform*>(m_arrayCombatTile[iEnableIndex]->Get_Component(ComTag_Transform))->Get_WorldMatrix();

		if (TRUE == static_cast<CSMesh*>(m_arrayCombatTile[iEnableIndex]->Get_Component(ComTag_SMesh))->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDis, matWorld))
		{
			_vec2 vUIPos = CUtility::World2Ui(m_arrayCombatTile[iEnableIndex]->Get_Position());
			
			m_pCharCombatUI->Set_Ui_PosX((_int)vUIPos.x);
			m_pCharCombatUI->Set_Ui_PosY((_int)vUIPos.y);
			m_pCharCombatUI->Set_Active(TRUE);

			m_iAnswer = -1;
			m_eCheck = E_CHECK::CHECK_CHAR_COMBAT;
			m_bCheck = TRUE;

			m_pSwapMascot = pSwapMascot;
		
			return TRUE;
		}

	}

	return FALSE;
}

void CTileField::Check_WeakTile()
{
	for (_int i = 0; i < m_iMaxArray; ++i)
	{

		if (E_TILE_OPTION::TILE_WEAK != m_arrayCombatTile[i]->Get_TileOption())
		{
			continue;
		}

		if (m_arrayCombatTile[i]->Get_Used())
		{
			m_arrayCombatTile[i]->Set_TileOption(E_TILE_OPTION::TILE_VARIABLE_OFF);
			m_arrayCombatTile[i]->Set_TileActive(FALSE);

			for (auto& pPlayer : m_vecPlayerMascot)
			{
				_int iIndex = pPlayer->Get_CurrentTileIndex();
				if (i == iIndex)
				{
					//타일이 무너져서 스타트타일로 이동
					pPlayer->Set_CurrentTileIndex(pPlayer->Get_StartIndex());
					pPlayer->Set_Pos(m_arrayCombatTile[pPlayer->Get_StartIndex()]->Get_Position());
				}
			}

			for (auto& pMonster : m_vecMonsterMascot)
			{
				_int iIndex = pMonster->Get_CurrentTileIndex();
				if (i == iIndex)
				{
					pMonster->Set_MascotDead(TRUE);
				}
			}
		}
	}

	for (auto& pPlayer : m_vecPlayerMascot)
	{
		_int iIndex = pPlayer->Get_CurrentTileIndex();
		if (m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_WEAK)
		{
			m_arrayCombatTile[iIndex]->Set_Used(TRUE);
		}
	}

	for (auto& pMonster : m_vecMonsterMascot)
	{
		_int iIndex = pMonster->Get_CurrentTileIndex();
		if (m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_WEAK)
		{
			m_arrayCombatTile[iIndex]->Set_Used(TRUE);
		}
	}
}

void CTileField::Check_Move()
{
	
	if (!m_bCheck)
	{
		return;
	}
	if (m_iMoveIndex < 0 || m_iMoveIndex >= m_iMaxArray)
	{
		return;
	}
	
	if (!m_pSelectMascot->Get_Move())
	{
		for (auto& pEnableMesh : m_vecEnableMesh)
		{
			pEnableMesh->Set_Active(FALSE);
		}

		m_bCheck = FALSE;

		if (m_pSwapMascot)
		{
			m_pSwapMascot->Set_CurrentTileIndex(m_pSelectMascot->Get_CurrentTileIndex());
			m_pSwapMascot = nullptr;
		}

		m_pSelectMascot->Set_CurrentTileIndex(m_iMoveIndex);

		_bool bTest = TRUE;

		if (m_pSelectMascot->Get_ObjTag() == E_OBJTAG::OBJ_Player)
		{
			if (Check_Tile())
			{
				bTest = FALSE;
			}
		}


		if (Check_Meeting())
		{
			bTest = FALSE;
		}

		if (bTest)
		{
			Select_NextPlayer();
		}
	}
}

_bool CTileField::Check_Tile()
{
	if (!m_pSelectMascot)
	{
		return FALSE;
	}

	_int iCurrentTileIndex = m_pSelectMascot->Get_CurrentTileIndex();

	E_TILE_OPTION eTileOption = m_arrayCombatTile[iCurrentTileIndex]->Get_TileOption();

	switch (eTileOption)
	{
	case Engine::E_TILE_OPTION::TILE_EMPTY:
		break;
	case Engine::E_TILE_OPTION::TILE_NORMAL:
		break;
	case Engine::E_TILE_OPTION::TILE_WARP_ONE_WAY_START:
		//알림창
		m_eCheck = E_CHECK::CHECK_WARP;
		Open_CheckUI(m_eCheck);
		return TRUE;
		break;
	case Engine::E_TILE_OPTION::TILE_WARP_ONE_WAY_END:
		break;
	case Engine::E_TILE_OPTION::TILE_WARP_TWO_WAY:
		//알림창
		m_eCheck = E_CHECK::CHECK_WARP;
		Open_CheckUI(m_eCheck);
		return TRUE;
		break;
	case Engine::E_TILE_OPTION::TILE_SPAWN:
		if (!m_arrayCombatTile[iCurrentTileIndex]->Get_Used())
		{
			m_arrayCombatTile[iCurrentTileIndex]->Set_Used(TRUE);
			m_arrayCombatTile[m_arrayCombatTile[iCurrentTileIndex]->Get_LinkTileIndex()]->Set_TileActive(TRUE);
			m_arrayCombatTile[m_arrayCombatTile[iCurrentTileIndex]->Get_LinkTileIndex()]->Set_TileOption(E_TILE_OPTION::TILE_VARIABLE_ON);
		}
		break;
	case Engine::E_TILE_OPTION::TILE_VARIABLE_OFF:
		break;
	case Engine::E_TILE_OPTION::TILE_REMOVE:
		if (!m_arrayCombatTile[iCurrentTileIndex]->Get_Used())
		{
			m_arrayCombatTile[iCurrentTileIndex]->Set_Used(TRUE);
			m_arrayCombatTile[m_arrayCombatTile[iCurrentTileIndex]->Get_LinkTileIndex()]->Set_TileActive(FALSE);
			m_arrayCombatTile[m_arrayCombatTile[iCurrentTileIndex]->Get_LinkTileIndex()]->Set_TileOption(E_TILE_OPTION::TILE_VARIABLE_OFF);

			for (auto& pMonster : m_vecMonsterMascot)
			{
				if (m_arrayCombatTile[iCurrentTileIndex]->Get_LinkTileIndex() == pMonster->Get_CurrentTileIndex())
				{
					pMonster->Set_MascotDead(TRUE);
				}
			}

			for (auto& pPlayer : m_vecPlayerMascot)
			{
				if (m_arrayCombatTile[iCurrentTileIndex]->Get_LinkTileIndex() == pPlayer->Get_CurrentTileIndex())
				{
					pPlayer->Set_CurrentTileIndex(pPlayer->Get_StartIndex());
					pPlayer->Set_Pos(m_arrayCombatTile[pPlayer->Get_StartIndex()]->Get_Position());
				}
			}
		}
		break;
	case Engine::E_TILE_OPTION::TILE_VARIABLE_ON:
		break;
	case Engine::E_TILE_OPTION::TILE_WEAK:
		break;
	case Engine::E_TILE_OPTION::TILE_START:
		break;
	case Engine::E_TILE_OPTION::TILE_END:
		break;
	default:
		break;
	}

	return FALSE;
}

_bool CTileField::Check_Meeting()
{
	if (!m_pSelectMascot)
	{
		return FALSE;
	}

	if (m_pSelectMascot->Get_ObjTag() == E_OBJTAG::OBJ_Player)
	{
		for (auto& pMonster : m_vecMonsterMascot)
		{
			if (pMonster->Get_MascotDead())
			{
				continue;
			}

			_int iMonsterTileIndex = pMonster->Get_CurrentTileIndex();
			if (iMonsterTileIndex == m_pSelectMascot->Get_CurrentTileIndex())
			{
				//전투 돌입
				CWorldCreator* pWorldCreator = pMonster->Get_WorldCreator();
				if (!pWorldCreator)
				{
					return FALSE;
				}

				CGameManager::GetInstance()->Set_NextWorld(pWorldCreator);
				CGameManager::GetInstance()->Set_CurrentSquadEnum(m_pSelectMascot->Get_Squad());
				m_bCombatSceneChange = TRUE;
				

				m_pWarMonster = pMonster;
				m_pWarPlayer = m_pSelectMascot;
				m_pSelectMascot = nullptr;
				return TRUE;
			}
		}
	}
	else
	{
		for (auto& pPlayer : m_vecPlayerMascot)
		{
			if (pPlayer->Get_MascotDead())
			{
				continue;
			}

			_int iPlayerTileIndex = pPlayer->Get_CurrentTileIndex();
			if (iPlayerTileIndex == m_pSelectMascot->Get_CurrentTileIndex())
			{
				//전투 돌입
				CWorldCreator* pWorldCreator = m_pSelectMascot->Get_WorldCreator();
				if (!pWorldCreator)
				{
					return FALSE;
				}

				CGameManager::GetInstance()->Set_NextWorld(pWorldCreator);
				CGameManager::GetInstance()->Set_CurrentSquadEnum(pPlayer->Get_Squad());
				m_bCombatSceneChange = TRUE;

				m_pWarPlayer = pPlayer;
				m_pWarMonster = m_pSelectMascot;
				m_pSelectMascot = nullptr;
				return TRUE;
			}
		}
	}

	return FALSE;
}

_bool CTileField::Action_Monster()
{
	if (!m_pSelectMascot)
	{
		return FALSE;
	}

	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Monster)
	{
		return FALSE;
	}
	
	
	switch (m_pSelectMascot->Get_PlayType())
	{
	case E_ENEMY_PLAY_TYPE::TYPE_AGGRESSIVE:
		return Action_Aggressive();
		break;
	case E_ENEMY_PLAY_TYPE::TYPE_TRACKING:
		return Action_Tracking();
		break;
	default:
		break;
	}

	return FALSE;

}

_bool CTileField::Action_Aggressive()
{
	if (!m_pSelectMascot)
	{
		return FALSE;
	}

	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Monster)
	{
		return FALSE;
	}

	_int iTileIndex = m_pSelectMascot->Get_CurrentTileIndex();

	for (auto& iNearIndex : m_arrayCombatTile[iTileIndex]->Get_NearTileIndex())
	{
		for (auto& pPlayer : m_vecPlayerMascot)
		{
			if (pPlayer->Get_MascotDead())
			{
				continue;
			}

			_int iPlayerIndex = pPlayer->Get_CurrentTileIndex();

			if (iNearIndex == iPlayerIndex)
			{
				_vec3 vTargetPos = m_arrayCombatTile[iPlayerIndex]->Get_Position();
				m_pSelectMascot->Set_MovePos(vTargetPos);
				m_bCheck = TRUE;
				m_eCheck = E_CHECK::CHECK_MOVE;
				m_iMoveIndex = iPlayerIndex;

				m_pSelectMascot->Set_AP(0);
				return TRUE;
			}
		}
	}

	return FALSE;
}
	
_bool CTileField::Action_Tracking()
{
	CMascot* pTargetPlayer = nullptr;
	pTargetPlayer = Search_Player();
	if (!pTargetPlayer)
	{

		return FALSE;
	}
	else
	{
		_int iResult = Find_Path(pTargetPlayer->Get_CurrentTileIndex());
		if (iResult < 0)
		{
			
			return FALSE;
		}

		for (auto& pMonsterMascot : m_vecMonsterMascot)
		{
			if (iResult == pMonsterMascot->Get_CurrentTileIndex())
			{
			
				return FALSE;
			}
		}

		if (!m_arrayCombatTile[iResult]->Get_TileActive())
		{
			return FALSE;
		}

		m_bCheck = TRUE;
		m_eCheck = E_CHECK::CHECK_MOVE;
		m_pSelectMascot->Set_MovePos(m_arrayCombatTile[iResult]->Get_Position());
		m_iMoveIndex = iResult;
		m_pSelectMascot->Set_AP(0);

		return TRUE;
	}

	return FALSE;
}

CMascot* CTileField::Search_Player()
{
	if (!m_pSelectMascot)
	{
		return nullptr;
	}
	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Monster)
	{
		return nullptr;
	}

	m_vecSearchedTile.clear();
	m_vecSearchTile.clear();

	m_vecSearchTile.emplace_back(m_pSelectMascot->Get_CurrentTileIndex());

	CMascot* pPlayer = nullptr;
	Search_Tile(pPlayer);
	
	return pPlayer;


}

void CTileField::Search_Tile(CMascot*& pPlayer)
{
	if (pPlayer)
	{
		return;
	}

	while (!m_vecSearchTile.empty())
	{
		for (auto& iSearchIndex : m_vecSearchTile)
		{
			for (auto& pPlayerMascot : m_vecPlayerMascot)
			{
				_int iPlayerIndex = pPlayerMascot->Get_CurrentTileIndex();

				if (iSearchIndex == iPlayerIndex)
				{
					pPlayer = pPlayerMascot;
					return;
				}
			}

		}

		vector<_int> m_vecNewSearchIndex;
		for (auto& iSearchedIndex : m_vecSearchTile)
		{
			m_vecSearchedTile.emplace_back(iSearchedIndex);
			for (auto& iNearIndex : m_arrayCombatTile[iSearchedIndex]->Get_NearTileIndex())
			{
				_bool bCheck = TRUE;
				for (auto& iSearched : m_vecSearchedTile)
				{
					if (iSearched == iNearIndex)
					{
						bCheck = FALSE;
						break;
					}
				}

				if (bCheck)
				{
					m_vecNewSearchIndex.emplace_back(iNearIndex);
				}

			}
		}
		m_vecSearchTile.clear();
		m_vecSearchTile = m_vecNewSearchIndex;
	}
	
}



void CTileField::Set_NearTileIndex(_int iTileIndex)
{
	if ( iTileIndex < 0 || iTileIndex >= m_iMaxArray)
	{
		return;
	}

	vector<_int> vecNearTileIndex;

	_int iIndexX = iTileIndex % m_iMaxX;
	_int iIndexZ = iTileIndex / m_iMaxX;

	_int iIndex;
	_int iTestIndexX;
	_int iTestIndexZ;

	iTestIndexZ = iIndexZ;
	iTestIndexX = iIndexX - 1;
	if (iTestIndexX >= 0)
	{
		iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
		if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
		{
			vecNearTileIndex.emplace_back(iIndex);
		}
	}

	iTestIndexZ = iIndexZ;
	iTestIndexX = iIndexX + 1;
	if (iTestIndexX < m_iMaxX)
	{
		iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
		if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
		{
			vecNearTileIndex.emplace_back(iIndex);
		}
	}

	iTestIndexZ = iIndexZ - 1;
	iTestIndexX = iIndexX;
	if (iTestIndexZ >= 0)
	{
		iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
		if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
		{
			vecNearTileIndex.emplace_back(iIndex);
		}
	}

	iTestIndexZ = iIndexZ + 1;
	iTestIndexX = iIndexX;
	if (iTestIndexZ < m_iMaxZ)
	{
		iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
		if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
		{
			vecNearTileIndex.emplace_back(iIndex);
		}
	}

	if (iIndexZ % 2 == 0)
	{
		iTestIndexZ = iIndexZ - 1;
		iTestIndexX = iIndexX - 1;

		if (iTestIndexZ >= 0 && iTestIndexX >= 0)
		{
			iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
			if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
			{
				vecNearTileIndex.emplace_back(iIndex);
			}
		}

		iTestIndexZ = iIndexZ + 1;
		iTestIndexX = iIndexX - 1;
		if (iTestIndexZ < m_iMaxZ && iTestIndexX >= 0)
		{
			iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
			if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
			{
				vecNearTileIndex.emplace_back(iIndex);
			}
		}
	}
	else
	{
		iTestIndexZ = iIndexZ - 1;
		iTestIndexX = iIndexX + 1;
		if (iTestIndexZ >= 0 && iTestIndexX < m_iMaxX)
		{
			iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
			if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
			{
				vecNearTileIndex.emplace_back(iIndex);
			}
		}

		iTestIndexZ = iIndexZ + 1;
		iTestIndexX = iIndexX + 1;
		if (iTestIndexZ < m_iMaxZ && iTestIndexX < m_iMaxX)
		{
			iIndex = iTestIndexZ * m_iMaxX + iTestIndexX;
			if (!(m_arrayCombatTile[iIndex]->Get_TileOption() == E_TILE_OPTION::TILE_EMPTY))
			{
				vecNearTileIndex.emplace_back(iIndex);
			}
		}
	}

	m_arrayCombatTile[iTileIndex]->Set_NearTileIndex(vecNearTileIndex);
	
}

void CTileField::Clear_DeadMascot()
{
	auto iterPlayer = m_vecPlayerMascot.begin();
	
	while (iterPlayer != m_vecPlayerMascot.end())
	{
		if ((*iterPlayer)->Get_Dead())
		{
			Safe_Release(*iterPlayer);
			iterPlayer = m_vecPlayerMascot.erase(iterPlayer);
		}
		else
		{
			++iterPlayer;
		}
	}

	auto iterMonster = m_vecMonsterMascot.begin();
	while (iterMonster != m_vecMonsterMascot.end())
	{
		if ((*iterMonster)->Get_Dead())
		{
			Safe_Release(*iterMonster);
			iterMonster = m_vecMonsterMascot.erase(iterMonster);
		}
		else
		{
			++iterMonster;
		}
	}
}

void CTileField::Open_CheckUI(E_CHECK eCheckUI)
{

	
	wstring wstrCheckMessage = L"";
	switch (eCheckUI)
	{
	case CTileField::CHECK_START:
		wstrCheckMessage = L"추가로 아군 부대를 배치할 수 있습니다.\n이 상태로 정말 시작하겠습니까?";
		break;
	case CTileField::CHECK_WARP:
		wstrCheckMessage = L"            해당 부대를 이동하시겠습니까?";
		break;
	case CTileField::CHECK_TURN_CHANGE:
		wstrCheckMessage = L"아직 행동하지 않은 아군 부대가 있습니다.\n정말 Phase를 종료하시겠습니까?";
		break;
	default:
		return;
		break;
	}
	m_pCheckUI->Get_ChildByIndex(1)->Set_Ui_Text(wstrCheckMessage);

	m_bCheck = TRUE;
	m_iAnswer = -1;
	m_pCheckUI->Set_Active(TRUE);


}

void CTileField::Set_EnemyInfo()
{
	vector<T_MONSTER_INFO>& pVecMonInfo = m_vecMonsterPool;

	CUI_Object* pEnenyInfo_Scroll = m_pTileFieldUI->Get_ChildByIndex(10)->Get_ChildByIndex(0)->Get_ChildByIndex(2);

	_int iMonSize = (_int)pVecMonInfo.size();
	_int iScrollChildSize = (_int)pEnenyInfo_Scroll->Get_ChildList()->size();

	// Monster정보를 표현할 UI 개수가 부족하면 새로 생성하고 자식으로 넣어준다.
	for (_int i = 0; i < iMonSize - iScrollChildSize; i++)
	{
		CUI_Object* pEnemyInfoPrefab = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]적정보_Prefab.uidat"); 
		pEnenyInfo_Scroll->Set_Child(pEnemyInfoPrefab);
	}

	iScrollChildSize = (_int)pEnenyInfo_Scroll->Get_ChildList()->size();
	for (_int i = 0; i < iScrollChildSize; ++i)
	{
		CUI_Object* pInfo = pEnenyInfo_Scroll->Get_ChildByIndex(i);
		if (i >= iMonSize)
		{
			pInfo->Set_Active(FALSE);
			continue;
		}

		T_MONSTER_INFO& tInfo = pVecMonInfo[i];
		wstring wstrImageName;
		wstrImageName = L"../../Reference/Resource/Texture/UI/EnemyInfo/";
		wstrImageName.append(tInfo.wszName);
		wstrImageName.append(L".png");
		pInfo->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Square_White.png");
		pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrImageName);
		if (tInfo.wszDisplayName != L"")
		{
			pInfo->Get_ChildByIndex(0)->Set_Ui_Text(tInfo.wszDisplayName);
		}
		else
		{
			pInfo->Get_ChildByIndex(0)->Set_Ui_Text(L"월드툴 이름 없음");
		}

		pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(TRUE);
		switch (tInfo.eMonsterType)
		{
		case Engine::E_MONSTER_TYPE::TYPE_NORMAL:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
			break;
		case Engine::E_MONSTER_TYPE::TYPE_ELITE:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/엘리트아이콘.png");
			break;
		case Engine::E_MONSTER_TYPE::TYPE_BOSS:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/보스아이콘.png");
			break;
		case Engine::E_MONSTER_TYPE::TYPE_END:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
			break;
		}
		pInfo->Get_ChildByIndex(1)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)tInfo.eAtkType, E_CHAR_ENUMTYPE::CNUM_ATKTYPE));
		pInfo->Get_ChildByIndex(2)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)tInfo.eDiffType, E_CHAR_ENUMTYPE::CNUM_DFFTYPE));

	}
}

void CTileField::Set_SquadWindow(CMascot* pPlayerMascot)
{
	if (!pPlayerMascot)
	{
		return;
	}
	if (pPlayerMascot->Get_ObjTag() != E_OBJTAG::OBJ_Player)
	{
		return;
	}

	E_SQUAD eSquad = pPlayerMascot->Get_Squad();

	m_pTileFieldUI->Get_ChildByIndex(1)->Set_Active(TRUE);
	m_pTileFieldUI->Get_ChildByIndex(7)->Set_Active(TRUE);

	m_pTileFieldUI->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Active(FALSE);
	m_pTileFieldUI->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(TRUE);

	_int iIndex = 0;
	for (auto& pPlayer : m_vecPlayerMascot)
	{
		if (pPlayer == pPlayerMascot)
		{
			break;
		}

		++iIndex;
	}

	WCHAR wszSquadName[16] = L"";
	swprintf_s(wszSquadName, L"%d부대", iIndex + 1);
	m_pTileFieldUI->Get_ChildByIndex(7)->Set_Ui_Text(wszSquadName);

	map<_int, vector<CCharacter*>>* pMapSquad = CGameManager::GetInstance()->Get_Squad(eSquad);

	CUI_Object* pSquadIndexUI = m_pTileFieldUI->Get_ChildByIndex(1);
	for (_int i = 0; i < 4; ++i)
	{
		CCharacter* pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
		if (!pChar)
		{
			pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(0)->Set_Active(FALSE);
			pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(1)->Set_Active(FALSE);
			pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(2)->Set_Active(TRUE);
			continue;
		}
		T_CharInfo tCharInfo = pChar->Get_CharInfo();
		CUI_Manager::GetInstance()->BattleFormation_SetWindow(pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(0), tCharInfo);

		//체력바 설정
		_float fPer = (_float)pChar->Get_UnitInfo().iNowHp / (_float)pChar->Get_UnitInfo().iMaxHp;
		pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_SizeX(_int(fPer*90.f));

		pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(0)->Set_Active(TRUE);
		pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(1)->Set_Active(TRUE);
		pSquadIndexUI->Get_ChildByIndex(2 + i)->Get_ChildByIndex(2)->Set_Active(FALSE);
	}

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];
		if (!pChar)
		{
			pSquadIndexUI->Get_ChildByIndex(6 + i)->Get_ChildByIndex(0)->Set_Active(FALSE);
			pSquadIndexUI->Get_ChildByIndex(6 + i)->Get_ChildByIndex(1)->Set_Active(TRUE);
			continue;
		}
		T_CharInfo tCharInfo = pChar->Get_CharInfo();
		CUI_Manager::GetInstance()->BattleFormation_SetWindow(pSquadIndexUI->Get_ChildByIndex(6 + i)->Get_ChildByIndex(0), tCharInfo);

		pSquadIndexUI->Get_ChildByIndex(6 + i)->Get_ChildByIndex(0)->Set_Active(TRUE);
		pSquadIndexUI->Get_ChildByIndex(6 + i)->Get_ChildByIndex(1)->Set_Active(FALSE);

	}
}

void CTileField::Release_PlayerMascot(CMascot * pMascot)
{
	if (!pMascot)
	{
		return;
	}
	if (pMascot->Get_ObjTag() != E_OBJTAG::OBJ_Player)
	{
		return;
	}

	auto iter = m_vecPlayerMascot.begin();
	while (iter != m_vecPlayerMascot.end())
	{
		if (*iter == pMascot)
		{
			(*iter)->Set_Dead(TRUE);
			Safe_Release(*iter);
			m_vecPlayerMascot.erase(iter);
			break;
		}

		++iter;
	}

	After_ReleasePlayer();

	
}

void CTileField::Release_PlayerMascot(E_SQUAD eSquad)
{

	auto iter = m_vecPlayerMascot.begin();
	while (iter != m_vecPlayerMascot.end())
	{
		if ((*iter)->Get_Squad() == eSquad)
		{
			(*iter)->Set_Dead(TRUE);
			Safe_Release(*iter);
			m_vecPlayerMascot.erase(iter);
			break;
		}

		++iter;
	}
	After_ReleasePlayer();

}

void CTileField::After_ReleasePlayer()
{
	WCHAR wszReadySquad[8] = L"";
	swprintf_s(wszReadySquad, L"%d/%d", (_int)m_vecPlayerMascot.size(), (_int)m_mapStartWorldUI.size());
	m_pTileFieldUI->Get_ChildByIndex(4)->Set_Ui_Text(wszReadySquad);

	if (m_vecPlayerMascot.empty())
	{
		m_pTileFieldUI->Get_ChildByIndex(1)->Set_Active(FALSE);
		m_pTileFieldUI->Get_ChildByIndex(7)->Set_Active(FALSE);

		m_pTileFieldUI->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Active(TRUE);
		m_pTileFieldUI->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(FALSE);
	}
	else
	{
		Set_SquadWindow(m_vecPlayerMascot.front());
	}
}

void CTileField::Check_Accept()
{
	m_iAnswer = 1;
	Close_CheckUI();
}

void CTileField::Check_Cancle()
{
	m_iAnswer = 0;
	Close_CheckUI();
}

void CTileField::Change_Squad()
{
	if (!m_bPlayerTurn)
	{
		return;
	}

	if (!m_pSelectMascot)
	{
		return;
	}
	if (m_pSelectMascot->Get_ObjTag() != E_OBJTAG::OBJ_Player)
	{
		return;
	}

	_int iIndex = 0;
	for (auto& pPlayer : m_vecPlayerMascot)
	{
		if (pPlayer == m_pSelectMascot)
		{
			++iIndex;
			continue;
		}
		if (pPlayer->Get_AP() > 0 || ((pPlayer->Get_WarpPoint() > 0)&& (m_arrayCombatTile[pPlayer->Get_CurrentTileIndex()]->Get_TileOption() == E_TILE_OPTION::TILE_WARP_ONE_WAY_START
			|| m_arrayCombatTile[pPlayer->Get_CurrentTileIndex()]->Get_TileOption() == E_TILE_OPTION::TILE_WARP_TWO_WAY)))
		{
			Select_Player(iIndex);
			
			break;
		}

		++iIndex;
	}
}

void CTileField::Change_TurnButton()
{
	if (!m_bPlayerTurn)
	{
		return;
	}

	_bool bAPCheck = FALSE;
	for (auto& pPlayer : m_vecPlayerMascot)
	{
		if (pPlayer->Get_AP() > 0)
		{
			bAPCheck = TRUE;
			break;
		}
	}

	if (!bAPCheck)
	{
		Call_TurnChange();
	}
	else
	{
		m_eCheck = CHECK_TURN_CHANGE;
		Open_CheckUI(m_eCheck);
	}
}

void CTileField::Change_AutoTurnChange()
{
	m_bAutoTurnChange = !m_bAutoTurnChange;
}

void CTileField::CharReady_Release()
{
	Release_PlayerMascot(m_pPickMascot);
	m_pPickMascot = nullptr;

	
	m_pCharReadyUI->Set_Active(FALSE);
	m_bCheck = FALSE;
}

void CTileField::CharReady_Swap()
{
	m_iSelectIndex = m_pPickMascot->Get_CurrentTileIndex();
	SetUIActive(FALSE);
	CUI_Manager::GetInstance()->Open_Ui_BattleFormation((_int)E_WORLD_MOD::MOD_MISSION);

	m_pCharReadyUI->Set_Active(FALSE);
	m_pPickMascot = nullptr;

	m_bCheck = FALSE;
}

void CTileField::CharReady_Trans()
{
	_int iIndex = 0;
	for (auto& tPair : m_mapStartWorldUI)
	{
		m_vecSelectTile[iIndex]->Set_Pos(m_arrayCombatTile[tPair.first]->Get_Position());
		m_vecSelectTile[iIndex]->Set_Active(TRUE);
		++iIndex;	
	}

	m_iAnswer = 0;
	m_pCharReadyUI->Set_Active(FALSE);
}

void CTileField::CharReady_Cancel()
{
	m_pCharReadyUI->Set_Active(FALSE);
	m_pPickMascot = nullptr;
	m_bCheck = FALSE;

}

void CTileField::CharCombat_Select()
{
	_int iIndex = 0;
	for (auto& pPlayer : m_vecPlayerMascot)
	{
		if (pPlayer == m_pSwapMascot)
		{
			break;
		}
		++iIndex;
	}

	Select_Player(iIndex);
	m_pCharCombatUI->Set_Active(FALSE);

	m_pSwapMascot = nullptr;
	m_bCheck = FALSE;
}

void CTileField::CharCombat_Trans()
{
	_vec3 vPos2 = m_pSelectMascot->Get_Position();
	_vec3 vPos = m_pSwapMascot->Get_Position();
	m_pSelectMascot->Set_MovePos(vPos);
	m_bCheck = TRUE;
	m_eCheck = E_CHECK::CHECK_MOVE;
	m_iMoveIndex = m_pSwapMascot->Get_CurrentTileIndex();

	m_pSwapMascot->Set_MovePos(vPos2);

	m_pCharCombatUI->Set_Active(FALSE);
}

void CTileField::CharCombat_Cancel()
{
	m_pSwapMascot = nullptr;
	m_pCharCombatUI->Set_Active(FALSE);
	m_bCheck = FALSE;
	m_bKeyDown = FALSE;
}

void CTileField::Check()
{
	switch (m_eCheck)
	{
	case CTileField::CHECK_STAGE_START:
		if (m_fUITimer + 1.f < CScene::Get_PlayTime())
		{
			m_pStageStart->Set_Active(FALSE);
			Call_TurnChange();
		}
		break;
	case CTileField::CHECK_TURN_END:
		if (m_fUITimer + 1.f < CScene::Get_PlayTime())
		{
			m_pMyPhase->Set_Active(FALSE);
			m_pEnemyPhase->Set_Active(FALSE);
			Change_Turn();
			m_bCheck = FALSE;
		}
		break;
	case CTileField::CHECK_MOVE:
		Check_Move();
		break;
	case CTileField::CHECK_START:
		if (m_iAnswer >= 0)
		{
			m_bCheck = FALSE;
			if (m_iAnswer == 1)
			{
				Start_Game();
			}
		}
		break;
	case CTileField::CHECK_WARP:
		if (m_iAnswer >= 0)
		{
			m_bCheck = FALSE;
			if (m_iAnswer == 1)
			{
				_int iIndex = m_arrayCombatTile[m_pSelectMascot->Get_CurrentTileIndex()]->Get_LinkTileIndex();
				m_pSelectMascot->Set_CurrentTileIndex(iIndex);
				m_pSelectMascot->Set_Pos(m_arrayCombatTile[iIndex]->Get_Position());

				m_pSelectMascot->Set_WarpPoint(0);

				if (m_pSelectMascot->Get_AP() > 0)
				{
					Enable_Check(m_pSelectMascot->Get_CurrentTileIndex());
					m_vecSelectTile.front()->Set_Pos(m_arrayCombatTile[iIndex]->Get_Position());
				}
				
			}

			if (m_pSelectMascot->Get_AP() < 1)
			{
				Select_NextPlayer();
			}
		}
		break;
	case CTileField::CHECK_TURN_CHANGE:
		if (m_iAnswer >= 0)
		{
			m_bCheck = FALSE;
			if (m_iAnswer == 1)
			{
				Call_TurnChange();
			}
		}
		break;
	case CTileField::CHECK_CHAR_READY:
		if (m_iAnswer >= 0)
		{
			if (m_iAnswer == 0)
			{
				if(CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
				{
					for (auto& tPair : m_mapStartWorldUI)
					{
						if (m_pPickMascot->Get_CurrentTileIndex() == tPair.first)
						{
							continue;
						}

						_int iStartTileIndex = tPair.first;
						_vec3 vPos;
						_float fDis;
						_matrix matWorld = static_cast<CTransform*>(m_arrayCombatTile[iStartTileIndex]->Get_Component(ComTag_Transform))->Get_WorldMatrix();

						if (TRUE == static_cast<CSMesh*>(m_arrayCombatTile[iStartTileIndex]->Get_Component(ComTag_SMesh))->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), &vPos, &fDis, matWorld))
						{
							for (auto& pPlayer : m_vecPlayerMascot)
							{
								if (pPlayer == m_pPickMascot)
								{
									continue;
								}
								if (pPlayer->Get_CurrentTileIndex() == iStartTileIndex)
								{
									pPlayer->Set_CurrentTileIndex(m_pPickMascot->Get_CurrentTileIndex());
									pPlayer->Set_StartIndex(m_pPickMascot->Get_StartIndex());
									pPlayer->Set_Pos(m_arrayCombatTile[m_pPickMascot->Get_StartIndex()]->Get_Position());
									break;
								}
							}

							m_pPickMascot->Set_CurrentTileIndex(iStartTileIndex);
							m_pPickMascot->Set_StartIndex(iStartTileIndex);
							m_pPickMascot->Set_Pos(m_arrayCombatTile[iStartTileIndex]->Get_Position());
					
						}
					}
					for (auto& pStaticObj : m_vecSelectTile)
					{
						pStaticObj->Set_Active(FALSE);
					}

					m_bCheck = FALSE;
					m_pPickMascot = nullptr;
				}
			}
		}
		break;
	case E_CHECK::CHECK_CHAR_COMBAT:
		
		break;
	case CTileField::CHECK_END:
		break;
	default:
		break;
	}

}

void CTileField::SetUIActive(_bool bActive)
{

	CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_BackGround)->front()->Set_Active(bActive);

	m_bUIActive = bActive;
	m_pTileFieldUI->Set_Active(bActive);
	for (auto& tPair : m_mapStartWorldUI)
	{
		tPair.second->Set_Active(bActive);
	}

	for (auto& pMonster : m_vecMonsterMascot)
	{
		pMonster->SetUIActive(bActive);
	}
}


_int CTileField::Find_Path(_int iTileIndex)
{
	m_listOpenNode.clear();
	m_listClosedNode.clear();

	T_TILENODE tStartNode;
	tStartNode.iTileIndex = m_pSelectMascot->Get_CurrentTileIndex();
	tStartNode.pParentTile = nullptr;
	tStartNode.fG = 0.f;
	tStartNode.fH = D3DXVec3Length(&(m_pSelectMascot->Get_Position() - m_arrayCombatTile[iTileIndex]->Get_Position()));
	tStartNode.fF = tStartNode.fG + tStartNode.fH;

	m_listOpenNode.emplace_back(tStartNode);

	return PathFinding(iTileIndex);
}

_int CTileField::PathFinding(_int iTargetIndex)
{

	T_TILENODE tMinTileNode;
	ZeroMemory(&tMinTileNode, sizeof(T_TILENODE));

	_bool bCheck = FALSE;
	_float fMinF = 0.f;
	
	list<T_TILENODE>::iterator iterMinF;

	if (m_listOpenNode.empty())
	{
		return -1;
	}
	for (auto iter = m_listOpenNode.begin(); iter != m_listOpenNode.end(); ++iter)
	{
		if (!bCheck)
		{
			fMinF = iter->fF;
			iterMinF = iter;
			bCheck = TRUE;
		}
		else
		{
			if (fMinF > iter->fF)
			{
				fMinF = iter->fF;
				iterMinF = iter;
			}
		}
	}

	tMinTileNode = *iterMinF;
	m_listClosedNode.emplace_back(tMinTileNode);
	m_listOpenNode.erase(iterMinF);

	list<T_TILENODE> listPathNode = Add_OpenList(&m_listClosedNode.back(), iTargetIndex);
	if (!listPathNode.empty())
	{
		return listPathNode.front().iTileIndex;
	}
	return PathFinding(iTargetIndex);
}

list<T_TILENODE> CTileField::Add_OpenList(T_TILENODE* pParentTileNode, _int iTargetIndex)
{

	list<T_TILENODE> listPathNode;
	_int iTileIndex = pParentTileNode->iTileIndex;
	for (auto& iNearTileIndex : m_arrayCombatTile[iTileIndex]->Get_NearTileIndex())
	{
		E_TILE_OPTION eTileOption = m_arrayCombatTile[iNearTileIndex]->Get_TileOption();
		if (eTileOption == E_TILE_OPTION::TILE_EMPTY)
		{
			continue;
		}

		_bool bTest = FALSE;

		T_TILENODE tNewTileNode;
		tNewTileNode.iTileIndex = iNearTileIndex;
		tNewTileNode.pParentTile = pParentTileNode;
		tNewTileNode.fG = pParentTileNode->fG + D3DXVec3Length(&(m_arrayCombatTile[iNearTileIndex]->Get_Position() - m_arrayCombatTile[iTileIndex]->Get_Position()));
		tNewTileNode.fH = D3DXVec3Length(&(m_arrayCombatTile[iNearTileIndex]->Get_Position() - m_arrayCombatTile[iTargetIndex]->Get_Position()));
		tNewTileNode.fF = tNewTileNode.fG + tNewTileNode.fH;

		if (iNearTileIndex == iTargetIndex)
		{
			
			while (tNewTileNode.pParentTile)
			{
				listPathNode.emplace_front(tNewTileNode);
				tNewTileNode = *tNewTileNode.pParentTile;
			}
			return listPathNode;
		}

		_bool bCheck = TRUE;
		for (auto iter = m_listClosedNode.begin(); iter != m_listClosedNode.end(); ++iter)
		{
			if (iter->iTileIndex == iNearTileIndex)
			{
				if (iter->fH > tNewTileNode.fH)
				{
					*iter = tNewTileNode;
				}
				bCheck = FALSE;
				break;
			}
		}

		if (bCheck)
		{
			m_listOpenNode.emplace_back(tNewTileNode);
		}


	}

	return listPathNode;
}





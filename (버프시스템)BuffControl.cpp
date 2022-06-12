// (개인) 마영전 모작을 할 당시 버프 시스템 컴포넌트의 클래스 
#include "stdafx.h"
#include "BuffControl.h"

#include "GameObject.h"
#include "CriticalEfx.h"
#include "Buff_Player.h"
#include "Tooltip_Stemina.h"
#include "Tooltip_Reinforce.h"

USING(Engine)

Engine::CBuffControl::CBuffControl(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT& eObjType)
	: CComponent(pGraphicDev)
{
}

Engine::CBuffControl::CBuffControl(const CBuffControl& rhs)
	: CComponent(rhs)
{
}

Engine::CBuffControl::~CBuffControl()
{
}

HRESULT Engine::CBuffControl::Ready_Component(OBJECT& eObjType)
{
	m_eObjectType = eObjType; 

	return S_OK;
}

_int CBuffControl::Update_Component(const _float & fTimeDelta)
{
	Compute_Buff(); 

	return _int();
}

void CBuffControl::Compute_Critical(const _matrix * matWorld)
{
	if (m_fCriticalPotential >= Engine::Random(0.f, 10.f))
	{
		Add_Buff(BUFF_CRITICAL, 1500);

		CCriticalEfx*	pEfx = static_cast<CCriticalEfx*>(Engine::Get_GameObject(L"Effect", L"Efx_Critical"));
		if (pEfx)
		{
			_vec3 vPos;
			memcpy(&vPos, &matWorld->_41, sizeof(_vec3));
			pEfx->Set_EnableCriticalEfx(vPos);
		}

		SoundMgrLowerVol(L"hit_common_critical.wav", CSoundMgr::BATTLE, 0.3f);
	}
}

void CBuffControl::Add_Buff(BUFF_ID eID, _ulong dwBuffDuration)
{
	list<tag_BuffDeBuff*>::iterator		iter = m_listBuff.begin();

	for (; iter != m_listBuff.end(); )
	{
		if (eID == (*iter)->eBuffID)
		{
			Safe_Delete(*iter);
			iter = m_listBuff.erase(iter);

			m_listBuff.emplace_back(new tag_BuffDeBuff(eID, GetTickCount(), dwBuffDuration));
			break;
		}
		else
		{
			++iter;
		}
	}

	if (iter == m_listBuff.end())
	{
		switch (eID)
		{
		case Engine::BUFF_CRITICAL:
			break;

		case Engine::BUFF_STAMINA:
			static_cast<CBuff_Player*>(Engine::Get_GameObject(L"Player_UI", L"Buff_Stemina_Icon"))->Set_BuffIcon(true);
			static_cast<CTooltip_Stemina*>(Engine::Get_GameObject(L"Player_UI", L"Tooltip_Stemina"))->Set_TooltipOn(true);
			break;

		case Engine::BUFF_REINFORCE:
			static_cast<CBuff_Player*>(Engine::Get_GameObject(L"Player_UI", L"Buff_Reinforce_Icon"))->Set_BuffIcon(true);
			static_cast<CTooltip_Reinforce*>(Engine::Get_GameObject(L"Player_UI", L"Tooltip_Reinforce"))->Set_TooltipOn(true);
			break;
		}

		m_listBuff.emplace_back(new tag_BuffDeBuff(eID, GetTickCount(), dwBuffDuration));
	}
}

void CBuffControl::Compute_Buff()
{
	if (!m_listBuff.empty())
	{
		list<tag_BuffDeBuff*>::iterator		iter = m_listBuff.begin();

		for (; iter != m_listBuff.end(); )
		{
			// Buff È¿°ú 
			switch ((*iter)->eBuffID)
			{
			case BUFF_CRITICAL:
				if (STATE_FURY != m_ePreState)
				{
					SetNextSmash(STATE_FURY, (*iter)->dwBuffDuration);
				}
				else
				{
					SetNextSmash(STATE_FURY2, (*iter)->dwBuffDuration);
				}

				//Safe_Delete(*iter);
				//iter = m_listBuff.erase(iter);
				break;

			case BUFF_STAMINA:
				m_tObjectInfo.fStaminaRecovery = m_tObjectInfo.fStaminaOriginRecovery * 2.5f;
				break;

			case BUFF_REINFORCE:
				m_tObjectInfo.fAttackSpeedInterpol = 0.25f;
				m_fCriticalPotential = 5.5f;
				break;
			}

			// Buff ½Ã°£ Á¾·á ½Ã »èÁ¦ 
			if ((*iter)->dwBuffStartTime + (*iter)->dwBuffDuration < GetTickCount())
			{
				switch ((*iter)->eBuffID)
				{
				case BUFF_CRITICAL:
					break;

				case BUFF_STAMINA:
					m_tObjectInfo.fStaminaRecovery = m_tObjectInfo.fStaminaOriginRecovery;

					if (OBJECT::OBJ_PLAYER == m_eObjectType)
					{
						static_cast<CBuff_Player*>(Engine::Get_GameObject(L"Player_UI", L"Buff_Stemina_Icon"))->Set_BuffIcon(false);
						static_cast<CTooltip_Stemina*>(Engine::Get_GameObject(L"Player_UI", L"Tooltip_Stemina"))->Set_TooltipOn(false);
					}
					break;

				case BUFF_REINFORCE:
					m_tObjectInfo.fAttackSpeedInterpol = 0.f;
					m_fCriticalPotential = m_fOriginCritical;

					if (OBJECT::OBJ_PLAYER == m_eObjectType)
					{
						static_cast<CBuff_Player*>(Engine::Get_GameObject(L"Player_UI", L"Buff_Reinforce_Icon"))->Set_BuffIcon(false);
						static_cast<CTooltip_Reinforce*>(Engine::Get_GameObject(L"Player_UI", L"Tooltip_Reinforce"))->Set_TooltipOn(false);
					}
					break;
				}

				Safe_Delete(*iter);
				iter = m_listBuff.erase(iter);
			}

			if (iter != m_listBuff.end())
			{
				++iter;
			}
		}
	}
}

void Engine::CBuffControl::Free()
{
	CComponent::Free();
}


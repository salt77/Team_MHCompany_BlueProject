#include "Boss_HpBar.h"

#include "UI_Manager.h"

CBoss_HpBar::CBoss_HpBar(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag)
	: CUI_Object(pGraphicDev, pHwnd, eObjTag)
{
}

CBoss_HpBar::CBoss_HpBar(const CBoss_HpBar & rhs)
	: CUI_Object(rhs)
{
}

CBoss_HpBar::~CBoss_HpBar()
{
}

CBoss_HpBar* CBoss_HpBar::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								 HWND * pHwnd,
								 const wstring& wstrBossName,
								 _int iBossMaxHp,
								 _uint iLineCount,
								 const wstring& wstrLoadFile,
								 E_OBJTAG eObjTag)
{
	CBoss_HpBar*	pInstance = new CBoss_HpBar(pGraphicDev, pHwnd, eObjTag);

	if (FAILED(pInstance->Ready_Object(wstrBossName, iBossMaxHp, iLineCount, wstrLoadFile)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_HpBar::Free()
{
	if (m_pBossHpBar_Parent)
	{
		m_pBossHpBar_Parent->Set_Dead(TRUE); 
		m_pBossHpBar_Parent = nullptr; 
	}

	CUI_Object::Free();
}

void CBoss_HpBar::Save(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Save(hFile, dwByte);
}

void CBoss_HpBar::Load(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Load(hFile, dwByte);
}

HRESULT CBoss_HpBar::Ready_Object(const wstring& wstrBossName, _int iBossMaxHp, _uint iLineCount, const wstring& wstrLoadFile)
{
	CUI_Object::Ready_Object();

	m_iBossMaxHp = iBossMaxHp;
	m_iBossHp = iBossMaxHp;
	m_iMaxLineCount = iLineCount;
	m_iCurLineCount = iLineCount;
	m_iLineHp = m_iBossMaxHp / m_iMaxLineCount;

	Load_SaveFile(wstrLoadFile);

	if (L"[Fixed]Boss_Hp_Bar_Normal.uidat" != wstrLoadFile)
	{
		m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Font/BossName_" + wstrBossName + L".png");

		if (L"Hieronymus" == wstrBossName)
		{
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeX((_int)(500 * 0.5));
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeY((_int)(72 * 0.5));
			m_pBossHpBar_Parent->Set_Ui_PosX(-300);
		}
		else if (L"Shiro" == wstrBossName)
		{
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeX((_int)(517 * 0.5));
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeY((_int)(165 * 0.5));
		}
		else if (L"Kuro" == wstrBossName)
		{
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeX((_int)(472 * 0.5));
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeY((_int)(160 * 0.5));
		}
		else if (L"Binah" == wstrBossName)
		{
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeX((_int)(472 * 0.5));
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeY((_int)(160 * 0.5));
		}
		else if (L"Hod" == wstrBossName)
		{
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeX((_int)(472 * 0.5));
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Set_Ui_SizeY((_int)(160 * 0.5));
		}

		m_pBossHpBar_Parent->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Portrait_Raidboss_" + wstrBossName + L".png");
	}
	else
	{
		for (_uint i = 0; i < 5; ++i)
		{
			m_pBossHpBar_Parent->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(wstrBossName);
		}
	}

	// 그로기 게이지
	m_pBossHpBar_GroggyGage = m_pBossHpBar_Parent->Get_ChildByIndex(1);
	m_pBossHpBar_GroggyGage->Set_ResetMask(FALSE);
	m_pBossHpBar_GroggyGage->Set_Mask(_vec2(0.f, 0.f), _vec2(0.f, 1.f));

	return S_OK;
}

_int CBoss_HpBar::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CUI_Object::Start_Object(fTimeDelta);

	return iExit;
}

_int CBoss_HpBar::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CUI_Object::Update_Object(fTimeDelta);

	if (!m_pBossHpBar_Parent)
	{
		return iExit;
	}

	Update_BossHpLerpBar(fTimeDelta);

	// 0->0 ~ 3(체력바들)
	m_pBossHpBar[(_uint)m_eCurBossHp]->Set_Mask(VEC2_ZERO, _vec2((_float)m_iLineHp / (m_iBossMaxHp / m_iMaxLineCount), 1.f));
	m_pBossHpBar_Lerp->Set_Mask(VEC2_ZERO, _vec2(m_fLerpBar, 1.f));

	// 1(남은 라인 Count Text)
	m_pBossHpBar_Parent->Get_ChildByIndex(2)->Set_Ui_Text(L"x" + to_wstring(m_iCurLineCount));

	// 2(남은 체력 Text)
	m_pBossHpBar_Parent->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(m_iBossHp) + L" / " + to_wstring(m_iBossMaxHp));

	// 3(보스 이름 Pic)

	// 4(보스 얼굴 Pic)

	// 5 ~ 7(특수능력? Pic)
	for (_uint i = 6; i < 9; ++i)
	{
		m_pBossHpBar_Parent->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	}
	m_pBossHpBar_Parent->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 1.f - m_fGageStat), _vec2(1.f, 0.f == m_fGageStat ? 0.f : 1.f));
	m_pBossHpBar_Parent->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 2.f - m_fGageStat), _vec2(1.f, 0.f == m_fGageStat ? 0.f : 1.f));
	m_pBossHpBar_Parent->Get_ChildByIndex(8)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 3.f - m_fGageStat), _vec2(1.f, 0.f == m_fGageStat ? 0.f : 1.f));

	return iExit;
}

_int CBoss_HpBar::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CUI_Object::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CBoss_HpBar::Render_Object(_int iPass)
{
	CUI_Object::Render_Object(iPass);
}

void CBoss_HpBar::Add_Component()
{
	CUI_Object::Add_Component();
}

void CBoss_HpBar::Set_ConstantTable()
{
	CUI_Object::Set_Constantable();
}

void CBoss_HpBar::Load_SaveFile(const wstring& wstrLoadFile)
{
	if (m_pBossHpBar_Parent)
	{
		return;
	}

	m_pBossHpBar_Parent = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/" + wstrLoadFile);
	assert(m_pBossHpBar_Parent); 

	// Parent를 자식으로 세팅하는게 좋음.
	static_cast<CUI_Object*>(this)->Set_ImageAlpha(0.f);
	Set_Child(m_pBossHpBar_Parent);
	m_eCurBossHp = E_BOSS_HP::BOSSHP_2;

	for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END; ++i)
	{
		// 1부터 3까지 체력바  (0은 러프바)
		m_pBossHpBar[i] = m_pBossHpBar_Parent->Get_ChildByIndex(0)->Get_ChildByIndex(i);
		m_pBossHpBar[i]->Set_ResetMask(FALSE);
	}
	for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END; ++i)
	{
		if (i != (_uint)m_eCurBossHp)
		{
			if (E_BOSS_HP::BOSSHP_0 == m_eCurBossHp)
			{
				if (i == (_uint)E_BOSS_HP::BOSSHP_END - 1)
				{
					m_pBossHpBar[i]->Set_Active(TRUE);

					for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END; ++i)
					{
						m_pBossHpBar[(_uint)E_BOSS_HP::BOSSHP_2]->Set_SortingOrder(TRUE);
					}
				}
				else
				{
					m_pBossHpBar[i]->Set_Active(FALSE);
				}
			}
			else
			{
				if (i == (_uint)m_eCurBossHp - 1)
				{
					m_pBossHpBar[i]->Set_Active(TRUE);
				}
				else
				{
					m_pBossHpBar[i]->Set_Active(FALSE);
				}
			}
		}
		else
		{
			m_pBossHpBar[i]->Set_Active(TRUE);
		}
	}

	m_pBossHpBar_Lerp = m_pBossHpBar_Parent->Get_ChildByIndex(0)->Get_ChildByIndex(3);

	m_pBossHpBar_Lerp->Set_ResetMask(FALSE);
	m_pBossHpBar_Lerp->Set_SortingOrder(TRUE);

	m_pBossHpBar_Parent->Set_NameTag(L"보스 체력바");
	m_pBossHpBar_Parent->Set_Active(TRUE);
}

void CBoss_HpBar::Update_BossHpLerpBar(const _float & fTimeDelta)
{
	if ((_float)m_iLineHp / (m_iBossMaxHp / (_float)m_iMaxLineCount) < m_fLerpBar)
	{
		m_fCountingTime += fTimeDelta;

		if (m_fLerpDelayTime <= m_fCountingTime)
		{
			m_fLerpBar -= m_fLerpSpeed * fTimeDelta;
		}
	}
	else
	{
		m_fCountingTime = 0.f;
		m_fLerpBar = (_float)m_iLineHp / (m_iBossMaxHp / (_float)m_iMaxLineCount);
	}
}

void CBoss_HpBar::Set_Gage_Groggy(const _float fGroggyRatio)
{
	m_pBossHpBar_GroggyGage->Set_Mask(_vec2(0.f, 0.f), _vec2(fGroggyRatio, 1.f));
}

void CBoss_HpBar::Add_Damage_ForUI(_int iDamage)
{
	assert(m_pBossHpBar_Parent);

	m_iBossHp += iDamage;

	if (m_iLineHp < -iDamage)
	{
		--m_iCurLineCount;
		m_iLineHp = (m_iBossMaxHp / m_iMaxLineCount) + (m_iLineHp + iDamage);
		m_fLerpBar = 1.f;

		m_pBossHpBar[(_uint)m_eCurBossHp]->Set_Mask(VEC2_ZERO, _vec2(1.f, 1.f));

		if (E_BOSS_HP::BOSSHP_0 == m_eCurBossHp)
		{
			m_eCurBossHp = E_BOSS_HP((_uint)E_BOSS_HP::BOSSHP_END - 1);

			for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END - 1; ++i)
			{
				m_pBossHpBar_Lerp->Set_SortingOrder(FALSE);
				m_pBossHpBar[(_uint)E_BOSS_HP::BOSSHP_END - 1]->Set_SortingOrder(FALSE);
			}

			m_pBossHpBar[(_uint)E_BOSS_HP::BOSSHP_END - 1]->Set_SortingOrder(FALSE);
		}
		else
		{
			m_eCurBossHp = (E_BOSS_HP)((_uint)m_eCurBossHp - 1);

			m_pBossHpBar_Lerp->Set_SortingOrder(TRUE);
		}

		// 
		if (1 >= m_iCurLineCount)
		{
			for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END; ++i)
			{
				if (i < (_uint)E_BOSS_HP::BOSSHP_END - 1)
				{
					m_pBossHpBar_Lerp->Set_SortingOrder(FALSE);
				}

				m_pBossHpBar[(_uint)m_eCurBossHp]->Set_SortingOrder(FALSE);

				if (i != (_uint)m_eCurBossHp)
				{
					m_pBossHpBar[i]->Set_Active(FALSE);
				}
			}
		}
		else
		{
			for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END; ++i)
			{
				if (i != (_uint)m_eCurBossHp)
				{
					if (E_BOSS_HP::BOSSHP_0 == m_eCurBossHp)
					{
						if (i == (_uint)E_BOSS_HP::BOSSHP_END - 1)
						{
							m_pBossHpBar[i]->Set_Active(TRUE);

							for (_uint i = 0; i < (_uint)E_BOSS_HP::BOSSHP_END; ++i)
							{
								m_pBossHpBar[(_uint)E_BOSS_HP::BOSSHP_2]->Set_SortingOrder(TRUE);
							}
						}
						else
						{
							m_pBossHpBar[i]->Set_Active(FALSE);
						}
					}
					else
					{
						if (i == (_uint)m_eCurBossHp - 1)
						{
							m_pBossHpBar[i]->Set_Active(TRUE);
						}
						else
						{
							m_pBossHpBar[i]->Set_Active(FALSE);
						}
					}
				}
				else
				{
					m_pBossHpBar[i]->Set_Active(TRUE);
				}
			}
		}
	}
	else
	{
		m_iLineHp += iDamage;
	}

	m_iBossHp = CUtility::Clamp_int(m_iBossHp, 0, m_iBossMaxHp);
	m_iCurLineCount = CUtility::Clamp_int(m_iCurLineCount, 0, m_iMaxLineCount);
}

void CBoss_HpBar::Add_Gage_OtherStat(_float fValue)
{
	assert(m_pBossHpBar_Parent);

	m_fGageStat += fValue;

	m_fGageStat = CUtility::Clamp_float(m_fGageStat, 0.f, 3.f);
}

void CBoss_HpBar::Set_Gage_OtherStat(const _float & fValue)
{
	assert(m_pBossHpBar_Parent);

	m_fGageStat = fValue;

	m_fGageStat = CUtility::Clamp_float(m_fGageStat, 0.f, 3.f);
}

#ifndef __Engine_Enum_H__
#define __Engine_Enum_H__

namespace Engine
{
	enum class E_ShaderPass				{ Base, Stage, Stage_Instance, DMesh, DMesh_Instance, Debug, RunMap, Alpha, PASS_END };
	
	enum class E_WINMODE				{ MODE_FULL, MODE_WIN };

	enum class E_BUFFERID				{ BUFFER_TRICOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_END };

	enum class E_INFO					{ INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum class E_ROTATION				{ ROT_X, ROT_Y, ROT_Z, ROT_END };
	enum class E_POSITION				{ POS_X, POS_Y, POS_Z, POS_END };
	enum class E_SCALE					{ SCALE_X, SCALE_Y, SCALE_Z, SCALE_END };

	enum class E_TEXTURE_TYPE			{ TEX_NORMAL, TEX_CUBE, TEX_END };

	enum class E_RENDER_ID				{ RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_BILLBOARD, RENDER_UI, RENDER_SHADOW, RENDER_EFFECT, RENDER_SHOWWINDOW, RENDER_END };
	enum class E_INSTANCE_RENDER_ID		{ RENDER_INSTANCE_NONALPHA, RENDER_INSTANCE_ALPHA, RENDER_INSTANCE_BILLBOARD, RENDER_INSTANCE_RENDER_END };

	enum class E_MOUSEKEYSTATE			{ DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum class E_MOUSEMOVESTATE			{ DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class E_MESH_TYPE				{ MESH_STATIC, MESH_DYNAMIC, MESH_NAVI, MESH_END };

	enum class E_NAVI_POINT				{ POINT_START, POINT_FINISH, POINT_END };
	enum class E_COMPARE				{ COMPARE_LEFT, COMPARE_RIGHT };

	enum class E_MinMax					{ Min, Max };

	enum class E_WIRE_RENDER			{ WIRE_RENDER_NORMAL, WIRE_RENDER_SELECTED, WIRE_RENDER_NONE, WIRE_RENDER_INTERPOL, WIRE_RENDER_END };

	enum class E_RGB					{ RGB_R, RGB_G, RGB_B, RGB_END };

	enum class E_Alignment_Vert			{ Align_Left, Align_Center_Vert, Align_Right };
	enum class E_Alignment_Hori			{ Align_Top, Align_Center_Hori, Align_Bottom };

	enum class E_MONSTER_TYPE			{ TYPE_NORMAL, TYPE_ELITE, TYPE_BOSS, TYPE_END };
	enum class E_WORLD_MOD				{ MOD_MISSION, MOD_WANTED, MOD_SPECIAL, MOD_TOTAL, MOD_END };
	enum class E_WORLD_TYPE				{ TYPE_STREET, TYPE_FIELD, TYPE_INDOOR, TYPE_END };
	enum class E_DIR_ANCHOR				{ E_LEFT, E_RIGHT, E_TOP, E_BOTTOM, E_CENTER_X, E_CENTER_Y, E_DIREND };
	enum class E_DIR					{ E_LEFT, E_RIGHT, E_TOP, E_BOTTOM, E_END };
	enum class E_LIGHT_MODE				{ E_Perspective, E_OrthoGraphic, E_END };

	//SYSTEM_ABSOLUTE : ��ƼŬ�� ȸ���� ���� ȸ������ ���� �̵� ������ 
	//SYSTEM_LOCAL_ROTATION    : ��ƼŬ ȸ���� ȸ������ ���� �̵� ��� ����
	enum class E_COORD_SYSTEM			{ SYSTEM_ABSOLUTE, SYSTEM_LOCAL_ROTATION, SYSTEM_LOCAL_NONE_ROTATION, SYSTEM_CENTER, SYSTEM_END };
	
	enum class E_UI_ShaderPassType		{ Normal, ScrollView_Child, Mask, FillCircle };
	enum class E_Geometry				{ E_VERTICAL, E_HORIZONTAL, E_GEOMETRY_END };
	enum class E_UiButtonEventType		{ E_REFLECT, E_FALSE, E_TRUE, E_POPUP, END };
	enum class E_UiFunctionType			{ OnClick, OnEnable, OnDisable, UiFunctionTypeEnd };
	
	enum class E_TextAlign				{ Left, Center, Right };
	enum class E_NumberFontType			{ White, Mini, Normal, Resist, Weak, E_End };

	enum class E_AnimationPlayType		{ Stop, Play_Once, Play_Loop };
	enum class E_EffectType				{ Once, Loop, Once_PingPong, Loop_PingPong, End };
	enum class E_EffectRenderType		{ Buffer, Mesh, End };


	enum class E_MAINCAM				{ MAINCAM_DYNAMIC, MAINCAM_TPS, MAINCAM_CAFE, MAINCAM_QUAT, MAINCAM_END };

	enum class E_AscendingType			{ ��������, �������� };
	enum class E_HIDE					{ HIDE_NONE, HIDE_SMALL, HIDE_BIG, HIDE_END };

	enum class E_UnitMode				{ None, Auto, TPS, NPC };

	enum class E_BattleSpeed			{ _X1, _X2, _X3 };

	enum class E_BOSS_HP				{ BOSSHP_0, BOSSHP_1, BOSSHP_2, BOSSHP_END };
	
	enum class E_RangeEffect			{ Circle_OutLine, RE_End	};
	enum class E_WarnType				{ Circle_Fill, Circle_Magic, Circle_InsideFill, Circle_OutDot, Circle_Wave, Circle_Lava, Circle_Lightning, WarnEnd };

	enum class E_COLTAG					{ COL_SPHERE, COL_END		};

	enum class E_GlowAnimMode			{ One, Loop, Pingpong		};

	enum class E_Combat_StatusType		{ Buff, CC, DeBuff, Special		};

	enum class E_Combat_Status			{ B_DEF, B_Dodge, B_DotHeal, B_HealEffectiveness, B_HealPower, B_HIT, B_MAXHP, B_Mind, B_MoveSpeed, B_OppressionPower, B_OppressionResist, B_Range, B_Shield, B_View,
										  Spe_Fury, Spe_Immortal, 
										  CC_Blind, CC_Charmed, CC_Confusion, CC_Emp, CC_Fear, CC_Inoperative, CC_MindControl, CC_Paralysis, CC_Provoke, CC_Slience, CC_Stunned,
										  DB_AmmoCount, DB_ATK, DB_AttackSpeed, DB_Bleed, DB_BlockRate, DB_BulletTypeDamage, DB_Burn, DB_Chill, DB_ConcentratedTarget, 
										  DB_CriticalChance, DB_CriticalChanceResistPoint, DB_CriticalDamage,
										  DB_DamagedRatio, DB_DamageRatio, DB_DEF, DB_Dodge, DB_DotHeal, DB_HealEffectiveness, DB_HealPower, DB_HIT, DB_MAXHP, DB_Mind, 
										  DB_MoveSpeed, DB_OppressionPower, DB_OppressionResist, DB_Range, DB_Shield, DB_View, 
										  Combat_StatusEnd };

	enum class E_EASE
	{
		InSine, OutSine, InOutSine,
		InCubic, OutCubic, InOutCubic,
		InQuint, OutQuint, InOutQuint,
		InCirc, OutCirc, InOutCirc,
		InQuad, OutQuad, InOutQuad,
		InQuart, OutQuart, InOutQuart,
		InExpo, OutExpo, InOutExpo,
		InBack, OutBack, InOutBack,
		None
	};

	enum class E_LAYERTAG
	{
		Environment, GameLogic, Monster, Character, Map, Obstacle,
		AttackObj_Player, AttackObj_Monster, Effect, Effect2, Damage_Font,
		UI, UI_Lobby, UI_Combat,
		TAG_END
	};

	enum class E_OBJTAG
	{
		OBJ_BackGround, OBJ_Test, OBJ_Test2,

		OBJ_DynamicCamera, OBJ_Camera_Cafe, OBJ_Camera_TPS, OBJ_Camera_Quat,
		OBJ_CamControl_Eye, OBJ_CamControl_At,

		OBJ_UI, OBJ_UI_SLIDER, OBJ_UI_SLIDER_BAR, OBJ_UI_SCROLLVIEW, OBJ_UI_NUMBER,
		OBJ_UI_BossHpbar,
		OBJ_Player, OBJ_Monster, OBJ_Special, OBJ_SWEEPER,

		OBJ_Map, OBJ_Obstacle, OBJ_CombatItem,

		OBJ_Particle, OBJ_Effect, OBJ_Trail,

		OBJ_CafeTile, OBJ_InviteChar,

		OBJ_BlockTile, OBJ_BlockTile_L, OBJ_BlockTile_R,

		OBJ_AtkObj,

		OBJ_Run_Player, OBJ_Run_Map, OBJ_Run_BossMonster, OBJ_Run_Monster, 

		OBJ_Halo,

		OBJ_END
	};

	enum class E_SCENETAG
	{
		Scene_LoadingScene,
		Scene_Logo,
		Scene_Lobby,
		Scene_Cafe,
		Scene_Combat,
		Scene_TileField,
		Scene_RunTest,
		Scene_END
	};

	//////////////////////////////////////////////////// ĳ���� ����ü�� �� �����͵� 
	// ù ��° ��Ʈ Data �� 
	enum class E_CHAR_NAME
	{
		Name_Neru, Name_Nonomi, Name_Nodoka, Name_Mashiro, Name_Maki, Name_Momoi, Name_Mutsuki, Name_Midori, Name_Saya, Name_Serina,
		Name_Serika, Name_Shun, Name_Sumire, Name_Suzumi, Name_Shiroko, Name_Shimiko, Name_Shizuko, Name_Aru, Name_Aris,
		Name_Asuna, Name_Ayane, Name_Airi, Name_Azusa, Name_Akane, Name_Akari, Name_Eimi, Name_Wakamo, Name_Yoshimi, Name_Utaha, Name_Yuuka,
		Name_Yuzu, Name_Iori, Name_Izuna, Name_Izumi, Name_Juri, Name_Zunko, Name_Tsurugi, Name_Tsubaki, Name_Chinatsu,
		Name_Chise, Name_Karin, Name_Kayoko, Name_Kotama, Name_Kotori, Name_Pina, Name_Hanae, Name_Hare, Name_Haruna,
		Name_Haruka, Name_Hasumi, Name_Hoshino, Name_Fuuka, Name_Hina, Name_Hibiki, Name_Hihumi,
		Name_End
	};
	enum class E_CHAR_LANG			{ LANG_ENG, LANG_KOR, LANG_END };

	enum class E_CHAR_STAR			{ STAR_NULL, STAR_1, STAR_2, STAR_3, STAR_END };

	enum class E_CHAR_COMBATTYPE	{ COMBATTYPE_NULL, COMBATTYPE_STRIKER, COMBATTYPE_SPECIAL, COMBATTYPE_END };

	enum class E_CHAR_ROLE			{ ROLL_NULL, ROLE_TANKER, ROLE_DEALER, ROLE_HEALER, ROLE_SUPPORTER, ROLE_END };

	enum class E_CHAR_POSITION		{ POSITION_NULL, POSITION_FRONT, POSITION_MIDDLE, POSITION_BACK, POSITION_END };

	enum class E_CHAR_ATKTYPE		{ ATKTYPE_NULL, ATKTYPE_EXPLOSION, ATKTYPE_PIERCE, ATKTYPE_MYSTERY, ATKTYPE_NORMAL, ATKTYPE_END };

	enum class E_CHAR_DFFTYPE		{ DFFTYPE_NULL, DFFTYPE_HEAVY, DFFTYPE_LIGHT, DFFTYPE_SPECICAL, DFFTYPE_END };

	enum class E_CHAR_MAPBUFF		{ MAPBUFF_NULL, MAPBUFF_SS, MAPBUFF_S, MAPBUFF_A, MAPBUFF_B, MAPBUFF_C, MAPBUFF_D, MAPBUFF_END };

	enum class E_CHAR_GRADE			{ GRADE_NULL, GRADE_FIRST, GRADE_SECOND, GRADE_THIRD, GRADE_END };

	enum class E_CHAR_SKILL_LV		{ SKILL_LV_NULL, SKILL_LV_1, SKILL_LV_2, SKILL_LV_3, SKILL_LV_4, SKILL_LV_5, SKILL_LV_END };

	enum class E_CHAR_WEAPONTYPE
	{
		WEAPONTYPE_NULL, WEAPONTYPE_SG, WEAPONTYPE_SMG, WEAPONTYPE_HG, WEAPONTYPE_GL, WEAPONTYPE_SR,
		WEAPONTYPE_MG, WEAPONTYPE_AR, WEAPONTYPE_MT, WEAPONTYPE_RL, WEAPONTYPE_RG,
		WEAPONTYPE_END
	};
	enum class E_CHAR_SCHOOL
	{
		// HYAKKIYAKO : ��;��� �����п�,	SHANHAIJING : ���ذ�,	 REDWINTER : �����ܿ� �����п� 
		SCHOOL_NULL, SCHOOL_ABYDOS, SCHOOL_TRINITY, SCHOOL_GEHENNA, SCHOOL_MILLENIUM, SCHOOL_HYAKKIYAKO,
		SCHOOL_SHANHAIJING, SCHOOL_REDWINTER,
		SCHOOL_END
	};

	//////////////////////////////////////////////////// ĳ���� ����ü���� ������ Enum�� 

	enum class E_CHAR_ENUMTYPE
	{
		CNUM_NAME, CNUM_KORNAME, CNUM_STAR, CNUM_COMBATTYPE, CNUM_ROLE,
		CNUM_POSITION, CNUM_ATKTYPE, CNUM_DFFTYPE, CNUM_WEAPONTYPE, CNUM_MAPBUFF,
		CNUM_SCHOOL, CNUM_GRADE, CNUM_OTHERS,
		CNUM_END
	};

	//////////////////////////////////////////////////// �л��ο��� ������ �� ���Ǵ� �з� ���  

	enum class E_CHAR_SORT
	{
		SORT_LEVEL, SORT_STAR, SORT_TIES, SORT_SCHOOL, SORT_NAME, SORT_EXSKILL_LV, SORT_SKILL_LV, SORT_INCITY_LV, SORT_OUTDOOR_LV,
		SORT_INDOOR_LV, SORT_ATKTYPE, SORT_DFFTYPE, SORT_ROLE, SORT_WEAPONTYPE, SORT_RANGE, SORT_GETTIME,
		SORT_REVERSE,
		SORT_END
	};

	//////////////////////////////////////////////////// ĳ���� ����ü�� ������ �ִ� ��� �����͵� 
	enum class E_CHAR_STRUCTALL
	{
		CHARINFO_bGetChar, CHARINFO_bHide,
		CHARINFO_eName, CHARINFO_eStar, CHARINFO_eCombatType, CHARINFO_eRole, CHARINFO_ePosition, CHARINFO_eAtkType, CHARINFO_eDffType,
		CHARINFO_eWeaponType, CHARINFO_eInCityBuff, CHARINFO_eOutDoorBuff, CHARINFO_eInDoorBuff, CHARINFO_eSchool, CHARINFO_eGrade,
		CHARINFO_wstrCircle, CHARINFO_wstrGeneralInfo, CHARINFO_iAge, CHARINFO_wstrBirthDay, CHARINFO_iHeight, CHARINFO_wstrHobby,
		CHARINFO_fExSkill, CHARINFO_fNormalSkill, CHARINFO_fReinforceSkill, CHARINFO_fSubSkill, CHARINFO_iExSkillCost,
		CHARINFO_fExSkill_Increase, CHARINFO_fNormalSkill_Increase, CHARINFO_fReinforceSkill_Increase, CHARINFO_fSubSkill_Increase,
		CHARINFO_iNowHP, CHARINFO_iMaxHP, CHARINFO_iAttack, CHARINFO_iDeffence, CHARINFO_iHeal, CHARINFO_iHitRatio,
		CHARINFO_iEvasion, CHARINFO_iCritical, CHARINFO_iCriticalDamage, CHARINFO_iStability, CHARINFO_iRange, CHARINFO_iCrowdReinforce, CHARINFO_iCrowdResist,
		CHARINFO_iCostHeal, CHARINFO_iIncLevel_HP, CHARINFO_iIncLevel_ATK, CHARINFO_iIncLevel_DEF, CHARINFO_iIncLevel_HEAL,
		CHARINFO_iLevel, CHARINFO_iExperience, CHARINFO_iMaxExperience,
		CHARINFO_END
	};

	/////////////////////////////////////////////////////

	enum class E_Pivot
	{
		Pivot_TL, Pivot_TC, Pivot_TR,
		Pivot_CL, Pivot_Center, Pivot_CR,
		Pivot_BL, Pivot_BC, Pivot_BR,
	};

	enum class E_Anchor
	{
		Anchor_TL, Anchor_TC, Anchor_TR,
		Anchor_CL, Anchor_Center, Anchor_CR,
		Anchor_BL, Anchor_BC, Anchor_BR,
	};

	enum class E_VertexOfAngle  /* ������ */
	{
		TL, TC, TR,
		CL, C, CR,
		BL, BC, BR
	};

	enum class E_Trigger
	{
		Attack_1, Attack_2, Attack_3, Dead, Reload, ExSkill, Buff_1, Buff_2,
		Trg_1, Trg_2, Trg_3, Trg_4, Trg_5, Trg_6, Trg_7, Trg_8, Trg_9,
		Trigger_End
	};

	// ��� ������ �±�
	enum class E_ItemTag
	{
		// ������
		�׷�_������, ����_������, ���÷�_������, ��Ű_������, ����Ű_������, ���_������, ������_������, ����ī_������,
		��_������, ���̷�_������, �����_������, �÷���_������, �ù���_������, ������_������, �Ʒ�_������, �ƽ���_������,
		�ƾ߳�_������, ���̸�_������, ���_������, ��ī��_������, ��ī��_������, ���̹�_������, ��ù�_������, ��Ÿ��_������,
		����ī_������, �̿���_������, �����_������, �ָ�_������, ����_������, �����_������, ����Ű_������, ġ����_������,
		ġ��_������, ī��_������, ī����_������, ��Ÿ��_������, ���丮_������, �ǳ�_������, �ϳ���_������, �Ϸ�_������,
		�Ϸ糪_������, �Ϸ�ī_������, �Ͻ���_������, ȣ�ó�_������, �Ŀ�ī_������, ����_������, ����Ű_������, ���Ĺ�_������,

		����_����������, ����_�ݴ��ǻ��, ����_��������, ����_��Ű��Ʈ, ����_õü������, ����_�����ķ�, ����_�������󹫳�,
		����_���������, ����_����Ʈ, ����_������,

		��ȭ��_���赵, ��ȭ��_�ϱ�, ��ȭ��_�Ϲ�, ��ȭ��_���, ��ȭ��_�ֻ��,


		// ���
		�Ʒγ���_������,
		����_�����Ʈ_���ذ�, �Ϲ�_�����Ʈ_���ذ�, ���_�����Ʈ_���ذ�, �ֻ��_�����Ʈ_���ذ�,
		����_�����Ʈ_�з��Ͼ�, �Ϲ�_�����Ʈ_�з��Ͼ�, ���_�����Ʈ_�з��Ͼ�, �ֻ��_�����Ʈ_�з��Ͼ�,
		����_�����Ʈ_�ƺ񵵽�, �Ϲ�_�����Ʈ_�ƺ񵵽�, ���_�����Ʈ_�ƺ񵵽�, �ֻ��_�����Ʈ_�ƺ񵵽�,
		����_�����Ʈ_���, �Ϲ�_�����Ʈ_���, ���_�����Ʈ_���, �ֻ��_�����Ʈ_���,
		����_�����Ʈ_Ʈ����Ƽ, �Ϲ�_�����Ʈ_Ʈ����Ƽ, ���_�����Ʈ_Ʈ����Ƽ, �ֻ��_�����Ʈ_Ʈ����Ƽ,
		����_�����Ʈ_��;���, �Ϲ�_�����Ʈ_��;���, ���_�����Ʈ_��;���, �ֻ��_�����Ʈ_��;���,
		����_�����Ʈ_�ܿ￬��, �Ϲ�_�����Ʈ_�ܿ￬��, ���_�����Ʈ_�ܿ￬��, �ֻ��_�����Ʈ_�ܿ￬��,

		����_��������BD_���ذ�, �Ϲ�_��������BD_���ذ�, ���_��������BD_���ذ�, �ֻ��_��������BD_���ذ�,
		����_��������BD_�з��Ͼ�, �Ϲ�_��������BD_�з��Ͼ�, ���_��������BD_�з��Ͼ�, �ֻ��_��������BD_�з��Ͼ�,
		����_��������BD_�ƺ񵵽�, �Ϲ�_��������BD_�ƺ񵵽�, ���_��������BD_�ƺ񵵽�, �ֻ��_��������BD_�ƺ񵵽�,
		����_��������BD_���, �Ϲ�_��������BD_���, ���_��������BD_���, �ֻ��_��������BD_���,
		����_��������BD_Ʈ����Ƽ, �Ϲ�_��������BD_Ʈ����Ƽ, ���_��������BD_Ʈ����Ƽ, �ֻ��_��������BD_Ʈ����Ƽ,
		����_��������BD_��;���, �Ϲ�_��������BD_��;���, ���_��������BD_��;���, �ֻ��_��������BD_��;���,
		����_��������BD_�ܿ￬��, �Ϲ�_��������BD_�ܿ￬��, ���_��������BD_�ܿ￬��, �ֻ��_��������BD_�ܿ￬��,

		����_Ȱ������, �Ϲ�_Ȱ������, ���_Ȱ������, �ֻ��_Ȱ������,

		����_�ϴϿ�_����, �ļյ�_����_�ϴϿ�, �����_����_�ϴϿ�, ������_����_�ϴϿ�,
		���̴�ġ_�纻_����, �ļյ�_���̴�ġ_�纻, �����_���̴�ġ_�纻, ������_���̴�ġ_�纻,
		��ƼŰ�׶�_��ġ_����, �ļյ�_��ƼŰ�׶�_��ġ, �����_��ƼŰ�׶�_��ġ, ������_��ƼŰ�׶�_��ġ,
		���׸�_����_����, �ļյ�_���׸�_����, �����_���׸�_����, ������_���׸�_����,
		��ȥġ_�纻_����, �ļյ�_��ȥġ_�纻, �����_��ȥġ_�纻, ������_��ȥġ_�纻,
		�������_����_����, �������_����, Ŀ�ٶ�_�������, ������_�������,
		�Է��_����_����, �ļյ�_�Է��_����, �����_�Է��_����, ������_�Է��_����,
		������ũ_ö����, ������ũ_ö����, ������_������ũ_ö����, �����_������ũ_ö����,
		�Ŀ����佺_����_����, �ļյ�_�Ŀ����佺_����, �����_�Ŀ����佺_����, ������_�Ŀ����佺_����,
		�׺��_��ũ_����, �ļյ�_�׺��_��ũ, �����_�׺��_��ũ, ������_�׺��_��ũ,

		�ν���_Ƽ��,

		// ����
		������ȸ_����, �ѷ���_����,
		// �Ҹ�ǰ
		������������BD_���ñ�, ���ʱ����Ʈ_���ñ�,
		ItemTag_End
	};


	// �����ۿ� ����ϴ� ��ȭ ����
	enum class E_Money				{ ������, �����׸�, ũ����, ����_�ѷ���, ����_������ȸ, û�ּ�, Money_End };

	enum class E_ItemFilter			{ ������, ����, ���, �Ҹ�ǰ, ����, ����ǰ, ��ȭ��, Filter_End };

	enum class E_ItemSortType		{ Basic, Name, Amount, Effect, E_End };

	enum class E_EquipKinds			{ Hat, Gloves, Shoes, Badge, Bag, Hairclip, Necklace, Charm, Watch, EquipKinds_End };

	enum class E_EquipType			{ EquipType_Attack, EquipType_HP, EquipType_Utility, EquipType_End };

	enum class E_EquipTier			{ Tier_1, Tier_2, Tier_3, Tier_4, Tier_5, Tier_6, Tier_End };

	enum class E_EquipTag
	{
		Hat_����ĸ����, Hat_��Ʈ�и���, Hat_������䵵��, Hat_����������, Hat_��ź���, Hat_�����̴���,
		Gloves_���������尩, Gloves_��Ʈ����尩, Gloves_��ηο����尩, Gloves_���ױ۷���, Gloves_��Ƽ�ñ۷���, Gloves_���̽��尩,
		Shoes_��ũ����Ŀ��, Shoes_��׺���, Shoes_��Ű��ī������, Shoes_��Ƽũ���������, Shoes_��������, Shoes_��������,
		Badge_�����ݼӹ���, Badge_����������Ʈ����, Badge_�ޱ׸��Ƶ�����������, Badge_���δ�ī�ڼ�����, Badge_ī���߸���ũ����ġ, Badge_���ڵ�������,
		Bag_�����������, Bag_���ѿ�ũ�ν���, Bag_��ηι���, Bag_���������, Bag_����������, Bag_��������Ʈ��,
		Hairclip_�״Ͻ������, Hairclip_����, Hairclip_��������, Hairclip_���������, Hairclip_�ٸ��������, Hairclip_�ڿ�������,
		Necklace_������������, Necklace_�����Ҵ�Ʈ, Necklace_���ݶ��̷���, Necklace_���ڰ���Ŀ, Necklace_������,
		Charm_�����������, Charm_�߿���, Charm_��η��Ǳ���, Charm_���ڰ�, Charm_ī���ö���ٷ縶,
		Watch_��������нð�, Watch_���׼ո�ð�, Watch_���̺�Ĺ�ո�ð�, Watch_��Ƽũȸ�߽ð�, Watch_�������ո�ð�,
		Equip_End
	};

	enum class E_FontType
	{
		���õ������_Medium, Noto_Sans_Medium, Noto_Sans_TC_Medium, RSU_Medium,
		���õ������_Bold, Noto_Sans_Bold, Noto_Sans_TC_Bold, RSU_Bold
	};

	enum class E_FurnitureTag
	{
		//[Default]
		//�ٴ�
		��������, ����������, ���������, ����������̺�, ����������̺�, �Ķ�������ī��,
		���򷲳����ٴ�,

		//��
		ȭ��Ʈ����, Ŀưâ��, ���򷲳�����,

		//[Momo]
		//�ٴ�
		�ٸ���Ÿ��η�����, ��������������Եȼ������̽�, ���������׿�������, ���ī���͹�, ����Űģī����,
		����������ἼƮ, �����η�����, ����̱�����, �����������, �÷��̴�ȭ��, �Ͼ�Ŀ�����̺�, �Ͼ����Ʈ���̺�,
		�Ķ��������̹ٴ�,

		//��
		���������޴������, �����������ڱ��, ȭ�м���, ��ημ�Ʈ����, ����â, �Ķ�Ÿ�Ϻ�,

		//[Gamecenter]
		//�ٴ�
		��̽�Ÿ�ǰ��ӱ�, ���̽����ӱ�, ������Ű���ӱ�, ī��Ʈ���������̺�, �����ƼƼ��, �÷�Ǯ���Ǳ�, ������ũ���ΰ���,
		��������̵���ӱ�, �Ķ��������̵���ӱ�, ����������̵���ӱ�, ������׿�������, �÷�Ǯ����ī��Ʈ, ��ũ����������,
		�����Ĺ�, ������ü���, �����̵�׿�������,
		��Ÿ�Ϲٴ�,

		//��
		����â��, ����Ÿ�Ϻ�,

		//[Valentine]
		//�ٴ�
		�߷�Ÿ��Ƽ���̺�, �߷�Ÿ����Ʈ��������, ���ݸ�����������, �������ݸ��Ŀ�ƾ, �߷�Ÿ����ũ����������, �߷�Ÿ�ν���������,
		�߷�Ÿ�ε���Ʈ������, �߷�Ÿ�ν�������ī����, �߷�Ÿ�ν���Ʈ������, �������ڹ���ī��, �߷�Ÿ�ν̱�����, ���ݸ���Ƽ��,
		�߷�Ÿ�ε���Ʈ�����̽�, �߷�Ÿ����Ű����, �߷�Ÿ�θ�Ƽ���̺�,
		�߷�Ÿ�κ�ȫ���ٴ�,

		//��
		����Ʈ������, ����Ʈ���ݸ������, ���ݸ��׿�������, �������ݸ�����, �߷�Ÿ�����ݸ�â��, �߷�Ÿ�����ݸ���,

		//[Poolparty]
		//�ٴ�
		ĳ���Ǹ���Ʈ����, �÷�Ǯ�Ѵ���Ǯ, Ʃ�갡������Ǯ, ��������Ʈ��Ʈ���̺�, ��Ʈ���ѻ�ܾ���, �����Ķ�ּ�Ʈ, ���̽ĸ���Ʈ���̺�,
		����Ʈǳȭ��, �Ͼ�����׶�ȭ��, ���������簢��ȭ��, Ǫ���������Ƽ��, �ٴٻ�å��Ʈ, ���̽��غ�����, ������Ʈ������ī��, ��Ʈ������ī��,
		�������Ʈ�������ٴ�,

		//��
		����Ʈǳâ��, �����׿�������, Ʈ�����ú����̾�Ʈ, ���ڹ���â��, Ʈ�����þ�Ʈ������, �����ĵ����̺�,

		//[Trophy]
		��Ʈ����_��, ��Ʈ����_�ǹ�, ��Ʈ����_���, ��Ʈ����_�÷�Ƽ��,
		�켼��Ʈ����_��, �켼��Ʈ����_�ǹ�, �켼��Ʈ����_���, �켼��Ʈ����_�÷�Ƽ��,
		�����δϹ���Ʈ����_��, �����δϹ���Ʈ����_�ǹ�, �����δϹ���Ʈ����_���, �����δϹ���Ʈ����_�÷�Ƽ��,
		ī����Ʈ����_��, ī����Ʈ����_�ǹ�, ī����Ʈ����_���, ī����Ʈ����_�÷�Ƽ��,
		�÷����Ʈ����_��, �÷����Ʈ����_�ǹ�, �÷����Ʈ����_���, �÷����Ʈ����_�÷�Ƽ��,

		Furniture_End
	};

	enum class E_FurnitureStar { STAR_1, STAR_2, STAR_3, STAR_END };

	enum class E_SkillType { Skill_Ex, Skill_Normal, Skill_Reinforce, Skill_Sub, SkillType_End };

	// ���̾�α� ���� 
	enum class E_Dialog_CharEvent
	{
		CharEvent_NULL,
		CharEvent_FadeIn, CharEvent_FadeOut,
		CharEvent_Updown_Shake, CharEvent_LeftRight_Shake,
		CharEvent_Beep,
		CharEvent_AllChar_FadeOut,
		CharEvent_End
	};
	enum class E_Dialog_PosChange
	{
		PosChange_NULL,
		PosChange_Center_Popup, PosChange_Left_Popup, PosChange_Right_Popup, PosChange_LC_Popup, PosChange_RC_Popup,
		PosChange_Center, PosChange_Left, PosChange_Right, PosChange_LC, PosChange_RC,
		PosChange_End
	};
	enum class E_Dialog_Background { Background_NoChange, Background_PpachinkoAlley, Background_End };
	enum class E_Dialog_ScreenEvent { ScreenEvent_NULL, ScreenEvent_FadeIn, ScreenEvent_FadeOut, ScreenEvent_End };
	enum class E_Dialog_Bgm { Bgm_ON, Bgm_OFF, Bgm_NC, Bgm_End };

	enum class E_Translation_Type { Translation_CharInfo, Translation_DialogInfo, Translation_End };

	enum class E_UiAnimationType
	{
		// ����� 0���� 1�� ���� �����ϴ� ��  
		// [�ʿ� ���� : �� �ð�, Ease]
		ZeroToOne_Vert, ZeroToOne_Hori, ZeroTo_Cross,

		// �¿�, ���Ϸ� ����  
		// [�ʿ� ���� : �� �ð�, ���� Ƚ��, ũ��]
		Vive_Vert, Vive_Hori,

		// ȭ��ۿ��� ���ƿ��� �ִϸ��̼�
		// [�ʿ� ���� : �� �ð�, Ease]
		Left_To_Origin, Right_To_Origin, Top_To_Origin, Bottom_To_Origin,

		UiAnim_End
	};

	enum class E_CHAR_ACTION
	{
		C_ACTION_IDLE, C_ACTION_MOVE, C_ACTION_STAND,
		C_ACTION_ATTACK, C_ACTION_RELOAD, C_ACTION_SKILL,
		C_ACTION_DEAD,
		C_ACTION_END
	};


	enum class E_ACTION
	{
		Normal_Idle, Normal_Shoot_Start, Normal_Shoot, Normal_Delay, Normal_Shoot_End,
		Normal_Reload, Normal_CallSign, Normal_Ready_Start, Normal_Ready_End, Move_End_Normal,

		Stand_Idle, Stand_Shoot_Start, Stand_Shoot, Stand_Delay, Stand_Shoot_End,
		Stand_Reload, Stand_CallSign, Stand_Ready_Start, Stand_Ready_End, Move_End_Stand,

		Sit_Idle, Sit_Shoot_Start, Sit_Shoot, Sit_Delay, Sit_Shoot_End,
		Sit_Reload, Sit_CallSign, Sit_Ready_Start, Sit_Ready_End, Move_End_Sit,

		Move_Ing, Move_CallSign, Jump,

		Vital_Death, Vital_Dying_Ing, Vital_Retreat, Vital_Panic,

		Formation_Idle, Formation_Pickup,

		ExSkill_CutIn, ExSkill,

		Victory_Start, Victory_End,

		Cafe_Idle, Cafe_Walk, Cafe_Reaction,

		Appear,

		Public01,

		Temp1, Temp2, Temp3, Temp4, Temp5,

		ACTION_END
	};

	enum class E_CharacterMouseTag
	{
		��������, ��¦���, ����, ����,
		�������, �ų�, ũ�ԼҸ�ħ, ȭ��,
		������, ������, ��ǥ��, �����,
		��Ȳ, ���, �������, ��¨��,
		MOUSE_END
	};


	enum class E_SQUAD
	{
		SQUAD_1, SQUAD_2, SQUAD_3, SQUAD_4,
		SQUAD_WANTED, SQUAD_SPECIAL, SQUAD_TOTAL,
		SQUAD_END
	};

	enum class E_TILE_OPTION
	{
		TILE_EMPTY,
		TILE_NORMAL,
		TILE_WARP_ONE_WAY_START, TILE_WARP_ONE_WAY_END,
		TILE_WARP_TWO_WAY,
		TILE_SPAWN, TILE_VARIABLE_OFF,
		TILE_REMOVE, TILE_VARIABLE_ON,
		TILE_WEAK,
		TILE_START,
		TILE_END
	};
	enum class E_ENEMY_PLAY_TYPE
	{
		TYPE_DEFENSIVE,
		TYPE_AGGRESSIVE,
		TYPE_TRACKING,
		TYPE_END
	};

	enum class E_ATKOBJ_TYPE
	{
		ATKOBJ_BULLET_NORMAL,
		ATKOBJ_SHIRO_BALL, ATKOBJ_SHIRO_BOMB_S, ATKOBJ_SHIRO_EX1,
		ATKOBJ_END
	};

	enum class E_FADETYPE
	{
		FADETYPE_IN, FADETYPE_OUT, FADETYPE_INOUT,
		FADETYPE_END
	};

	enum class E_DIALOG_LOCK
	{
		DIALOG_BRANCH, DIALOG_FADEINOUT,
		DIALOG_END
	};

	enum class E_CHESED_ATKTYPE
	{
		CHESED_ATK_NORMAL, CHESED_ATK_SPREE, CHESED_ATK_DEPLOY, CHESED_ATK_ROUNDED, 
		CHESED_ATK_END 
	};

	enum class E_DRONE_ATKTYPE
	{
		DRONE_ROTATE, DRONE_CHANGE, DRONE_ROUNDED, 
		DRONE_END 
	};

	enum class E_TANMAKTYPE
	{
		TANMAK_NORMAL, TANMAK_DESTROY, TANMAK_SLOW, 
		TANMAK_END
	};

	enum class E_RUN_BULLETTYPE 
	{
		BULLET_RED, BULLET_PINK, BULLET_BLUE, BULLET_YELLOW, 
		BULLET_END 
	};
	
	// ������ ���ݿ� �¾����� �߻��ϴ� ����Ʈ�� ���� �̳��Դϴ�.
	enum class E_UNIT_HIT_TYPE
	{
		HIT_NOTYPE_DEFAULT,
		HIT_AR_DEFAULT,
		HIT_SG_DEFAULT,
		HIT_MG_DEFAULT,
		HIT_HG_DEFAULT,
		HIT_SMG_DEFAULT,
		HIT_MT_DEFAULT,
		HIT_RG_DEFAULT,
		HIT_GL_DEFAULT,

		HIT_EXS,
		HIT_END
	};

	enum class E_BUFFDEBUFF_TYPE 
	{
		BUFF_ATK_UP, 
		BUFFDEBUFF_END 
	};


}

#endif // __Engine_Enum_H__

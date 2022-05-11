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

	//SYSTEM_ABSOLUTE : 파티클의 회전은 적용 회전값에 따른 이동 미적용 
	//SYSTEM_LOCAL_ROTATION    : 파티클 회전과 회전값에 따른 이동 모두 적용
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

	enum class E_AscendingType			{ 내림차순, 오름차순 };
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

	//////////////////////////////////////////////////// 캐릭터 구조체에 들어갈 데이터들 
	// 첫 번째 시트 Data 값 
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
		// HYAKKIYAKO : 백귀야행 연합학원,	SHANHAIJING : 산해경,	 REDWINTER : 붉은겨울 연방학원 
		SCHOOL_NULL, SCHOOL_ABYDOS, SCHOOL_TRINITY, SCHOOL_GEHENNA, SCHOOL_MILLENIUM, SCHOOL_HYAKKIYAKO,
		SCHOOL_SHANHAIJING, SCHOOL_REDWINTER,
		SCHOOL_END
	};

	//////////////////////////////////////////////////// 캐릭터 구조체에서 가지는 Enum들 

	enum class E_CHAR_ENUMTYPE
	{
		CNUM_NAME, CNUM_KORNAME, CNUM_STAR, CNUM_COMBATTYPE, CNUM_ROLE,
		CNUM_POSITION, CNUM_ATKTYPE, CNUM_DFFTYPE, CNUM_WEAPONTYPE, CNUM_MAPBUFF,
		CNUM_SCHOOL, CNUM_GRADE, CNUM_OTHERS,
		CNUM_END
	};

	//////////////////////////////////////////////////// 학생부에서 정렬할 때 사용되는 분류 방식  

	enum class E_CHAR_SORT
	{
		SORT_LEVEL, SORT_STAR, SORT_TIES, SORT_SCHOOL, SORT_NAME, SORT_EXSKILL_LV, SORT_SKILL_LV, SORT_INCITY_LV, SORT_OUTDOOR_LV,
		SORT_INDOOR_LV, SORT_ATKTYPE, SORT_DFFTYPE, SORT_ROLE, SORT_WEAPONTYPE, SORT_RANGE, SORT_GETTIME,
		SORT_REVERSE,
		SORT_END
	};

	//////////////////////////////////////////////////// 캐릭터 구조체가 가지고 있는 모든 데이터들 
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

	enum class E_VertexOfAngle  /* 꼭짓점 */
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

	// 모든 아이템 태그
	enum class E_ItemTag
	{
		// 엘레프
		네루_엘레프, 노노미_엘레프, 마시로_엘레프, 마키_엘레프, 무츠키_엘레프, 사야_엘레프, 세리나_엘레프, 세리카_엘레프,
		슌_엘레프, 스미레_엘레프, 스즈미_엘레프, 시로코_엘레프, 시미코_엘레프, 시즈코_엘레프, 아루_엘레프, 아스나_엘레프,
		아야네_엘레프, 아이리_엘레프, 이즈나_엘레프, 아카네_엘레프, 아카리_엘레프, 에이미_엘레프, 요시미_엘레프, 우타하_엘레프,
		유우카_엘레프, 이오리_엘레프, 이즈미_엘레프, 주리_엘레프, 준코_엘레프, 츠루기_엘레프, 츠바키_엘레프, 치나츠_엘레프,
		치세_엘레프, 카린_엘레프, 카요코_엘레프, 코타마_엘레프, 코토리_엘레프, 피나_엘레프, 하나에_엘레프, 하레_엘레프,
		하루나_엘레프, 하루카_엘레프, 하스미_엘레프, 호시노_엘레프, 후우카_엘레프, 히나_엘레프, 히비키_엘레프, 히후미_엘레프,

		선물_에이팟프로, 선물_금단의사랑, 선물_게임잡지, 선물_쿠키세트, 선물_천체망원경, 선물_전투식량, 선물_말차맛라무네,
		선물_젤리즈쿠션, 선물_오피트, 선물_곰인형,

		강화석_설계도, 강화석_하급, 강화석_일반, 강화석_상급, 강화석_최상급,


		// 재료
		아로나의_스탬프,
		기초_기술노트_산해경, 일반_기술노트_산해경, 상급_기술노트_산해경, 최상급_기술노트_산해경,
		기초_기술노트_밀레니엄, 일반_기술노트_밀레니엄, 상급_기술노트_밀레니엄, 최상급_기술노트_밀레니엄,
		기초_기술노트_아비도스, 일반_기술노트_아비도스, 상급_기술노트_아비도스, 최상급_기술노트_아비도스,
		기초_기술노트_게헨나, 일반_기술노트_게헨나, 상급_기술노트_게헨나, 최상급_기술노트_게헨나,
		기초_기술노트_트리니티, 일반_기술노트_트리니티, 상급_기술노트_트리니티, 최상급_기술노트_트리니티,
		기초_기술노트_백귀야행, 일반_기술노트_백귀야행, 상급_기술노트_백귀야행, 최상급_기술노트_백귀야행,
		기초_기술노트_겨울연방, 일반_기술노트_겨울연방, 상급_기술노트_겨울연방, 최상급_기술노트_겨울연방,

		기초_전술교육BD_산해경, 일반_전술교육BD_산해경, 상급_전술교육BD_산해경, 최상급_전술교육BD_산해경,
		기초_전술교육BD_밀레니엄, 일반_전술교육BD_밀레니엄, 상급_전술교육BD_밀레니엄, 최상급_전술교육BD_밀레니엄,
		기초_전술교육BD_아비도스, 일반_전술교육BD_아비도스, 상급_전술교육BD_아비도스, 최상급_전술교육BD_아비도스,
		기초_전술교육BD_게헨나, 일반_전술교육BD_게헨나, 상급_전술교육BD_게헨나, 최상급_전술교육BD_게헨나,
		기초_전술교육BD_트리니티, 일반_전술교육BD_트리니티, 상급_전술교육BD_트리니티, 최상급_전술교육BD_트리니티,
		기초_전술교육BD_백귀야행, 일반_전술교육BD_백귀야행, 상급_전술교육BD_백귀야행, 최상급_전술교육BD_백귀야행,
		기초_전술교육BD_겨울연방, 일반_전술교육BD_겨울연방, 상급_전술교육BD_겨울연방, 최상급_전술교육BD_겨울연방,

		기초_활동보고서, 일반_활동보고서, 상급_활동보고서, 최상급_활동보고서,

		수정_하니와_조각, 파손된_수정_하니와, 마모된_수정_하니와, 온전한_수정_하니와,
		보이니치_사본_조각, 파손된_보이니치_사본, 마모된_보이니치_사본, 온전한_보이니치_사본,
		안티키테라_장치_조각, 파손된_안티키테라_장치, 마모된_안티키테라_장치, 온전한_안티키테라_장치,
		에테르_조각_조각, 파손된_에테르_조각, 마모된_에테르_조각, 온전한_에테르_조각,
		로혼치_사본_조각, 파손된_로혼치_사본, 마모된_로혼치_사본, 온전한_로혼치_사본,
		만드라고라_씨앗_조각, 만드라고라_새싹, 커다란_만드라고라, 온전한_만드라고라,
		님루드_렌즈_조각, 파손된_님루드_렌즈, 마모된_님루드_렌즈, 온전한_님루드_렌즈,
		볼프세크_철조각, 볼프세크_철광석, 저순도_볼프세크_철조각, 고순도_볼프세크_철조각,
		파에스토스_원반_조각, 파손된_파에스토스_원반, 마모된_파에스토스_원반, 온전한_파에스토스_원반,
		네브라_디스크_조각, 파손된_네브라_디스크, 마모된_네브라_디스크, 온전한_네브라_디스크,

		부스터_티켓,

		// 코인
		전술대회_코인, 총력전_코인,
		// 소모품
		기초전술교육BD_선택권, 기초기술노트_선택권,
		ItemTag_End
	};


	// 아이템에 사용하는 재화 종류
	enum class E_Money				{ 엘레프, 엘리그마, 크레딧, 코인_총력전, 코인_전술대회, 청휘석, Money_End };

	enum class E_ItemFilter			{ 엘레프, 코인, 재료, 소모품, 선물, 수집품, 강화석, Filter_End };

	enum class E_ItemSortType		{ Basic, Name, Amount, Effect, E_End };

	enum class E_EquipKinds			{ Hat, Gloves, Shoes, Badge, Bag, Hairclip, Necklace, Charm, Watch, EquipKinds_End };

	enum class E_EquipType			{ EquipType_Attack, EquipType_HP, EquipType_Utility, EquipType_End };

	enum class E_EquipTier			{ Tier_1, Tier_2, Tier_3, Tier_4, Tier_5, Tier_6, Tier_End };

	enum class E_EquipTag
	{
		Hat_무지캡모자, Hat_니트털모자, Hat_빅브라더페도라, Hat_리본베레모, Hat_방탄헬멧, Hat_프릴미니햇,
		Gloves_스포츠용장갑, Gloves_니트벙어리장갑, Gloves_페로로오븐장갑, Gloves_가죽글러브, Gloves_택티컬글러브, Gloves_레이스장갑,
		Shoes_핑크스니커즈, Shoes_어그부츠, Shoes_핑키파카슬리퍼, Shoes_앤티크에나멜로퍼, Shoes_전술부츠, Shoes_힐펌프스,
		Badge_서벌금속배지, Badge_마나슬루펠트배지, Badge_앵그리아델리원형배지, Badge_베로니카자수배지, Badge_카제야마벨크로패치, Badge_코코데빌배지,
		Bag_방수스포츠백, Bag_방한용크로스백, Bag_페로로백팩, Bag_감색스쿨백, Bag_전술란도셀, Bag_데빌윙토트백,
		Hairclip_테니스헤어밴드, Hairclip_헤어슈슈, Hairclip_모모헤어핀, Hairclip_날개헤어핀, Hairclip_다목적헤어핀, Hairclip_코우모리헤어핀,
		Necklace_블루투스목걸이, Necklace_눈꽃팬던트, Necklace_니콜라이로켓, Necklace_십자가초커, Necklace_군번줄,
		Charm_교통안전부적, Charm_발열팩, Charm_페로로의깃털, Charm_십자가, Charm_카모플라쥬다루마,
		Watch_방수디지털시계, Watch_가죽손목시계, Watch_웨이브캣손목시계, Watch_엔티크회중시계, Watch_방진형손목시계,
		Equip_End
	};

	enum class E_FontType
	{
		경기천년제목_Medium, Noto_Sans_Medium, Noto_Sans_TC_Medium, RSU_Medium,
		경기천년제목_Bold, Noto_Sans_Bold, Noto_Sans_TC_Bold, RSU_Bold
	};

	enum class E_FurnitureTag
	{
		//[Default]
		//바닥
		원목의자, 원목보조의자, 원목수납장, 원목수납테이블, 원목대형테이블, 파랑색심플카펫,
		내츄럴나무바닥,

		//벽
		화이트보드, 커튼창문, 내츄럴나무벽,

		//[Momo]
		//바닥
		바리스타페로로인형, 모모프렌즈굿즈가포함된수납케이스, 모모프렌즈네온전광판, 우드카운터바, 나무키친카운터,
		원목수납음료세트, 배색페로로의자, 배색싱글의자, 배색더블의자, 컬러이단화분, 하얀커피테이블, 하얀디저트테이블,
		파란색폴리싱바닥,

		//벽
		모모프렌즈메뉴벽장식, 모모프렌즈액자기둥, 화분선반, 페로로세트선반, 유리창, 파란타일벽,

		//[Gamecenter]
		//바닥
		댄싱스타의게임기, 레이스게임기, 에어하키게임기, 카세트테이프테이블, 배색멀티티비, 컬러풀자판기, 젤리즈크레인게임,
		레드아케이드게임기, 파랑색아케이드게임기, 보라색아케이드게임기, 젤리즈네온전광판, 컬러풀패턴카페트, 핑크색심플의자,
		관엽식물, 흰색심플소파, 아케이드네온전광판,
		모눈타일바닥,

		//벽
		작은창문, 패턴타일벽,

		//[Valentine]
		//바닥
		발렌타인티테이블, 발렌타인하트선물상자, 초콜릿스위츠모형, 대형초콜릿파운틴, 발렌타인핑크스위츠모형, 발렌타인스위츠오븐,
		발렌타인디저트조리대, 발렌타인스위츠바카운터, 발렌타인스위트수납바, 갈색격자무늬카펫, 발렌타인싱글의자, 초콜릿파티션,
		발렌타인디저트쇼케이스, 발렌타인쿠키소파, 발렌타인멀티테이블,
		발렌타인분홍색바닥,

		//벽
		포인트조명기둥, 스위트초콜릿벽장식, 초콜릿네온전광판, 수제초콜릿모형, 발렌타인초콜릿창문, 발렌타인초콜릿벽,

		//[Poolparty]
		//바닥
		캐노피리조트소파, 컬러풀한대형풀, 튜브가들어간소형풀, 여름디저트세트테이블, 레트로한삼단앰프, 여름파라솔세트, 접이식리조트테이블,
		리조트풍화분, 하얀색몬스테라화분, 심플한직사각형화분, 푸른색우드파티션, 바다산책세트, 접이식해변의자, 원형스트라이프카펫, 스트라이프카펫,
		블루포인트포세린바닥,

		//벽
		리조트풍창문, 여름네온전광판, 트로피컬벽걸이아트, 격자무늬창문, 트로피컬아트프레임, 여름파도무늬벽,

		//[Trophy]
		비나트로피_동, 비나트로피_실버, 비나트로피_골드, 비나트로피_플래티넘,
		헤세드트로피_동, 헤세드트로피_실버, 헤세드트로피_골드, 헤세드트로피_플래티넘,
		히에로니무스트로피_동, 히에로니무스트로피_실버, 히에로니무스트로피_골드, 히에로니무스트로피_플래티넘,
		카이텐트로피_동, 카이텐트로피_실버, 카이텐트로피_골드, 카이텐트로피_플래티넘,
		시로쿠로트로피_동, 시로쿠로트로피_실버, 시로쿠로트로피_골드, 시로쿠로트로피_플래티넘,

		Furniture_End
	};

	enum class E_FurnitureStar { STAR_1, STAR_2, STAR_3, STAR_END };

	enum class E_SkillType { Skill_Ex, Skill_Normal, Skill_Reinforce, Skill_Sub, SkillType_End };

	// 다이얼로그 관련 
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
		// 사이즈가 0에서 1로 점차 증가하는 것  
		// [필요 변수 : 총 시간, Ease]
		ZeroToOne_Vert, ZeroToOne_Hori, ZeroTo_Cross,

		// 좌우, 상하로 진동  
		// [필요 변수 : 총 시간, 진동 횟수, 크기]
		Vive_Vert, Vive_Hori,

		// 화면밖에서 날아오는 애니메이션
		// [필요 변수 : 총 시간, Ease]
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
		떨떠름함, 살짝놀람, 싫음, 웃음,
		기분좋음, 신남, 크게소리침, 화남,
		일자입, 세모입, 무표정, 비웃음,
		당황, 놀람, 고양이입, 언짢음,
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
	
	// 유닛이 공격에 맞았을때 발생하는 이펙트의 설정 이넘입니다.
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

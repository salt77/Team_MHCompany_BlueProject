#ifndef __Engine_Struct_H__
#define __Engine_Struct_H__


namespace Engine
{
	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPosition;
		unsigned long	dwColor;
	}VTXCOL;

	const unsigned long	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	const  unsigned long	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexTerrain
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
		D3DXVECTOR3		vTangent;
	}VTXTERRAIN;

	const  unsigned long	FVF_TERRAIN = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;

	typedef struct tagVertexTRAIL
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
		D3DXVECTOR2		vRatio;
	}VTXTRAIL;

	const  unsigned long	FVF_TRAIL = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;

	}VTXCUBE;

	const  unsigned long	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);	// �ؽ��� UV�� FLOAT�� 3���� ������ ǥ���ϰڴٴ� �ɼ�

	typedef struct tagVertexUI
	{
		_vec3		vPosition;		// 12
		_vec2		vTexUV;			// 8
	}VTXUI;

	const _ulong	FVF_UI = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexTest
	{
		_vec3		vPosition;		// 12
		_vec3		vNormal;		// 12
		_vec2		vTexUV;			// 8
	}VTXTEST;

	const _ulong	FVF_Test = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	typedef struct tagVertexDMesh
	{
		_vec4		vPosition;		// 16
		_vec4		vNormal;		// 16
		_vec2		vTexUV;			// 8
		_vec4		vBlendWeight;   // 16
		_vec4		vBlendIndices;  // 16
	}VTX_DMESH;

	typedef struct tagVertex_Screen
	{
		D3DXVECTOR4		vPosition;
		D3DXVECTOR2		vTexUV;

	}VTXSCREEN;

	const unsigned long FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1; // RHW ������ ������ ��ġ ���� ǥ���ϴ� �ڷ���

	typedef struct tagVertexSphere
	{
		_vec3        vPosition;
		_vec3        vNormal;
		_vec2        vTexUV;

	}VTX_SPHERE;

	const _ulong    FVF_SPHERE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct INDEX16
	{
		unsigned short		_0;
		unsigned short		_1;
		unsigned short		_2;
	}INDEX16;

	typedef struct INDEX32
	{
		unsigned long		_0;
		unsigned long		_1;
		unsigned long		_2;
	}INDEX32;


	// �븻 ���Ϳ� ���ؽ� ����
	const D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,	0 },
		D3DDECL_END()
	};

	//======================= �ν��Ͻ��� ���ؽ� ���� =================

	//====�ν��Ͻ� �� ���ؽ� ����====
	const D3DVERTEXELEMENT9 vertexStaticDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },
		D3DDECL_END()
	};

	typedef struct tagInstanceData
	{
		_vec4   vColor;
		_matrix	matWorld;
	}T_INSTANCE;

	typedef struct tagInstanceParticleData
	{
		_vec4   vColor;
		_matrix	matWorld;
		_vec4   vUV;
	}T_PARTICLE_INSTANCE;

	const D3DVERTEXELEMENT9 declMesh[] =
	{
		{ 0, 0,      D3DDECLTYPE_FLOAT3,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		{ 0, 12,	 D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_NORMAL,	0 },
		{ 0, 24,	 D3DDECLTYPE_FLOAT2,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
		{ 1, 0,		 D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     0 },
		{ 1, 16,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     1 },
		{ 1, 32,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     2 },
		{ 1, 48,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     3 },
		{ 1, 64,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     4 },
		D3DDECL_END()
	};

	const D3DVERTEXELEMENT9 declRect[] =
	{
		{ 0, 0,      D3DDECLTYPE_FLOAT3,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		{ 0, 12,	 D3DDECLTYPE_FLOAT2,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
		{ 1, 0,		 D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     0 },
		{ 1, 16,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     1 },
		{ 1, 32,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     2 },
		{ 1, 48,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     3 },
		{ 1, 64,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     4 },
		{ 1, 80,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     5 },
		D3DDECL_END()
	};

	/*vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

	float4		vBlendWeights : BLENDWEIGHT;
	float4		vBlendIndices : BLENDINDICES;

	vector		vColor : COLOR0;
	vector		vRight : COLOR1;
	vector		vUp : COLOR2;
	vector      vLook : COLOR3;
	vector		vPos : COLOR4;*/

	const D3DVERTEXELEMENT9 declDMesh[] =
	{
		{ 0, 0,      D3DDECLTYPE_FLOAT4,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		{ 0, 16,	 D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_NORMAL,	0 },
		{ 0, 32,	 D3DDECLTYPE_FLOAT2,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
		{ 0, 40,	 D3DDECLTYPE_FLOAT4,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 56,	 D3DDECLTYPE_FLOAT4,    D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDINDICES,  0 },
		{ 1, 0,		 D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     0 },
		{ 1, 16,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     1 },
		{ 1, 32,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     2 },
		{ 1, 48,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     3 },
		{ 1, 64,     D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR,     4 },
		D3DDECL_END()
	};

	//==============================================================

	typedef struct tagParticleSpawnerInfo
	{
		E_COORD_SYSTEM eCoordSystem;

		float fEntranceTime;

		float fStartTime;
		float fLifeTime;

		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vRandomMinAddPos;
		D3DXVECTOR3 vRandomMaxAddPos;

		D3DXVECTOR3 vRot;
		D3DXVECTOR3 vRandomMinAddRot;
		D3DXVECTOR3 vRandomMaxAddRot;

		int	  iSpawnSampleIndex;
		float fSpawnTime;
		float fSpawnInterval;
		D3DXVECTOR3 vRandomMinSpawnPos;
		D3DXVECTOR3 vRandomMaxSpawnPos;

		bool	bMultiSpawn;
		int		iMultiSpawnCount;
		D3DXVECTOR3 vStartPos;
		D3DXVECTOR3 vStartRot;

		D3DXVECTOR3 vAddPos;
		D3DXVECTOR3 vAddRot;

		D3DXVECTOR3 vRandomMinSpawnRot;
		D3DXVECTOR3 vRandomMaxSpawnRot;
		_float fTemp[12];
	}T_PARTICLE_SPAWNER;

	typedef struct tagParticleInfo
	{
		E_COORD_SYSTEM eCoordSystem;

		float	fStartTime;
		float	fLifeTime;

		D3DXVECTOR3	  vPos;
		D3DXVECTOR3   vRandomMinAddPos;
		D3DXVECTOR3   vRandomMaxAddPos;

		D3DXVECTOR3   vRot;
		D3DXVECTOR3   vRandomMinAddRot;
		D3DXVECTOR3   vRandomMaxAddRot;

		D3DXVECTOR3   vScale;
		D3DXVECTOR3   vEndScale;
		D3DXVECTOR3	  vRenderScale;

		D3DXVECTOR4   vRenderColor;
		D3DXVECTOR4   vStartColor;
		D3DXVECTOR4   vEndColor;
		
		_int		  iTextureIndex;

		float		 fGravity;
		float        fMaxGravity;
		float        fCurrentGravity;

		_bool		 bBillBoard;
		
		//
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vRight;
		D3DXVECTOR3 vUp;

		_bool		bIndependent;
		D3DXMATRIX  matParentWorld;

		_bool       bBlackBackGroundImage;

		D3DXVECTOR4 vStartColorMax;
		D3DXVECTOR3	vOffSetRot;
		D3DXVECTOR3 vScaleMax;
		_float      fBillboardZRot;
		_bool		bBillBoardLookDir;
		_bool		bUseCross;
		_bool       bBlur;
		_bool		bZEnable;
		_bool       bTemp[4];
		_int        iAlphaOption;
		_int        iSpriteIndex;
		_int		iSpriteX;
		_int		iSpriteY;

		_float		fSpriteWidth;
		_float		fSpriteHeight;

		_float		fCycle;

		_float		fSpriteTimer;
		_float		fMaxLifeTime;
		_float		fTemp;
	}T_PARTICLE;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		D3DXMATRIX				CombinedTransformMatrix;
		bool					isCloned;
	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;
		LPDIRECT3DTEXTURE9*		ppNormalTexture;
		LPDIRECT3DTEXTURE9*		ppSpecularTexture;
		LPDIRECT3DTEXTURE9*		ppMaskTexture;

		LPD3DXMESH				pOriMesh;		// ��Ű���� ����Ǳ� �� ���� �ε���¸� �����ϱ� ���� �İ�ü

		unsigned long			dwNumBones;		// �޽��� ���� ���� ����(�޽��� ���� �� ������ ����� ������ �ľ��ϱ� ����)

		D3DXMATRIX*				pFrameOffSetMatrix;		// �ִϸ��̼��� ������ ���·� ����� ���� ���� �ִϸ��̼� ���

		D3DXMATRIX**			ppCombinedTransformMatrix;	// ���� ���� CombinedTransformMatrix ���� �ּҰ��� �����ϱ� ���� ���� ������
		D3DXMATRIX*				pRenderingMatrix;			// ���� ��� = pFrameOffSetMatrix * (*ppCombinedTransformMatrix);
		LPDIRECT3DVERTEXDECLARATION9 pDecl;

		LPD3DXBUFFER			pBoneCombinationBuf;
		LPD3DXATTRIBUTERANGE	pAttributeTable; // �޽��� �Ӽ� ���̺� ����ü.
		DWORD					NumAttributeGroups; // �Ӽ� ����.

		DWORD					dwMaxVertInfluences;
		bool					bMouse = FALSE;
		bool					bFace = FALSE;
		
		float					fBaseGlow = 0.f; // �⺻���� �Դ� �۷ο� ��		0.f(��������) ~ 1.f(����)
		float					fTexGlowR = 0.f;
		float					fTexGlowG = 0.f;
		float					fTexGlowB = 0.f;
		float					fGlowPower = 1.f; // �۷ο� ������				0.f(ǥ������ ����) ~ 1.f(������ ǥ��)


		float					fGlow_U_Min = 0.f;
		float					fGlow_U_Max = 1.f;

		float					fGlow_V_Min = 0.f;
		float					fGlow_V_Max = 1.f;

		_vec3					vGlowColor = _vec3(1.f, 1.f, 1.f);

	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagNaviMesh
	{
		tagNaviMesh()
		{
			ZeroMemory(vPos, sizeof(D3DXVECTOR3) * 3);
		}
		D3DXVECTOR3 vPos[3];
	}T_NAVIMESH;

	typedef struct tagTEXINFO
	{
		LPDIRECT3DTEXTURE9 pTexture; // HBITMAP ?? 

		D3DXIMAGE_INFO tImageInfo; // �̹����� ���� ������ ������ ���� ���� . 
	}T_TEXINFO;

	typedef struct tagRenderFont
	{
		tagRenderFont(wstring pTag, const wstring& pString, const _vec2& vPos, D3DXCOLOR color, UINT _iWidth, UINT _iHeight, UINT _iWeight, BOOL _bItalic)
			: szFontTag(pTag)
			, szString(pString)
			, vPosition(vPos)
			, Color(color)
			, iWidth(_iWidth)
			, iHeight(_iHeight)
			, iWeight(_iWeight)
			, bItalic(_bItalic)
		{

		}

		wstring szFontTag;
		const wstring szString;
		const _vec2 vPosition;
		D3DXCOLOR Color;
		UINT iWidth;
		UINT iHeight;
		UINT iWeight;
		BOOL bItalic;
	}T_RENDERFONT;

	typedef struct tagStFont
	{
		tagStFont(const _tchar* fontName, _uint width, _uint height, _uint weight)
			: iWidth(width), iHeight(height), iWeight(weight)
		{
			FontName = fontName;
		}
		tagStFont(const tagStFont& other)
			: iWidth(other.iWidth), iHeight(other.iHeight), iWeight(other.iWeight)
		{
			FontName = other.FontName;
		}
		wstring FontName;
		_uint iWidth;
		_uint iHeight;
		_uint iWeight;
	}T_FONT;

	typedef struct tagTextOption
	{
		E_FontType			eFontType;
		_int				iFontSize = 15;
		E_VertexOfAngle		eTextVertexOfAngle = E_VertexOfAngle::C;
		D3DXCOLOR			tColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		_int				iTextPosX = 0;
		_int				iTextPosY = 0;
		_bool				bBold = FALSE;
	}T_TextOption;

	typedef struct tagUITransform
	{
		_float Get_SizeFloat(E_POSITION ePos)
		{
			if (E_POSITION::POS_X == ePos)
			{
				return (iSizeX * fSizeRatioX);
			}
			else if (E_POSITION::POS_Y == ePos)
			{
				return (iSizeY * fSizeRatioY);
			}
			return 0.f;
		}
		_vec2				vDrawUvMin = _vec2(0.f, 0.f);
		_vec2				vDrawUvMax = _vec2(1.f, 1.f);
		_int				iPosX = 0;
		_int				iPosY = 0;
		_int				iPosZ = 0;
		_int				iSizeX = 100;
		_int				iSizeY = 100;
		_int				iSorting_Layer = 0;	// 0���� �� �׷���
		_int				iOrder_In_Layer = 0;	// Sorting_Layer�� ���� ��
		_float				fImageAlpha = 1.f;
		_bool				bUseText = FALSE;
		_int				iButtonEventType = (_int)E_UiButtonEventType::E_REFLECT;
		_bool				bAnimationState = FALSE;
		_float				fSizeRatioX = 1.f;
		_float				fSizeRatioY = 1.f;
		E_Pivot				eTransformPivot = E_Pivot::Pivot_Center;
		E_EASE				eEase = E_EASE::InOutCirc;
	}T_UiTransform;

	typedef struct tagUIAnimation
	{
		E_UiAnimationType eAnimType = E_UiAnimationType::UiAnim_End;
		_float	fProgNow = 0.f;
		_float	fProgMax = 1.f;
		_float	float_Parameter_1 = 0.f;
		_float	float_Parameter_2 = 0.f;
		_int	int_Parameter_1 = 10;
		_int	int_Parameter_2 = 50;

		_vec2	vAnimPosition = _vec2(0.f, 0.f);
		_int	iRandDir = 1;
		_bool	bMyChildFollowMyAnim = TRUE;
	}T_UiAnimation;

	typedef struct tagDateInfo
	{
		_int iYear;
		_int iMonth;
		_int iDay;
		_int iHour;
		_int iMinute;
		_int iSecond;
	}T_DATEINFO;

	typedef struct tagUserInfo
	{
		enum E_BaseExp
		{
			E_Character = 10,
			E_Equip = 10,
			E_Like = 10,
		};
		_int	Get_Level_MaxEXP(_int iInputLevel)
		{
			return ((_int)(E_BaseExp::E_Character * 1.5f * iInputLevel + (iInputLevel / 3)) + 30);
		}
		_int	Get_LikeLevel_MaxEXP(_int iInputLevel)
		{
			return (_int)(E_BaseExp::E_Like * iInputLevel);
		}
		_int	Get_EquipLevel_MaxEXP(_int iInputLevel)
		{
			return (_int)(E_BaseExp::E_Equip * iInputLevel);
		}

		//������ ���� �ð�
		tagDateInfo tDateInfo;
		//����
		_uint iLevel = 8;
		//�г���
		_tchar wszNickName[32] = L"�α��� �ʿ�";
		//����ġ
		_uint iUserEXP = 0;
		//ũ����
		_uint iCredit = 250350;
		//�����׸�
		_uint iEligma = 10000;
		//AP��
		_uint iAP = 0;
		//û�ּ�
		_uint iBluePyroxene = 0; 

		//	ĳ���� ���� ����
		_bool	bCharacter[(_int)E_CHAR_NAME::Name_End] = { 0, };

		//	ĳ���� ��� ��������
		_bool	bEquip[(_int)E_EquipType::EquipType_End];
		_uint	iEquip_Level[(_int)E_EquipType::EquipType_End];

		//	ĳ���� ��ų ����
		_uint	iChar_SkillLevel[(_int)E_CHAR_LANG::LANG_END][(_int)E_SkillType::SkillType_End] = { 0, };
		_uint	iChar_StarLevel[(_int)E_CHAR_LANG::LANG_END] = { 0, };

		//  �κ� ĳ���� ����
		E_CHAR_NAME	eLobby_Char = E_CHAR_NAME::Name_Neru;
		_int	iBossScore = 0;
		// �����


		// �ɼ� ����
		_float	fBGM_Ratio = 0.5f;
		_float	fFX_Ratio = 0.5f;
		_float	fVoice_Ratio = 0.5f;
		// �ٸ� �ɼǵ� ���� �߰�

		//��Ÿ ����

		//�κ��丮


		//������ ���� �Ⱓ
		//�������� Ŭ���� ����
		//���� ��ȯ ����
		//���� ���� ���� Ƚ�� 
	}T_USERINFO;

	typedef struct tagUnitInfo
	{
		// ���� 
		_int				iNowHp			= 1000;			// ���� ü��
		_int				iMaxHp			= 1000;			// �ִ� ü��
		_int				iAttack			= 100;			// ���ݷ�
		_int				iDeffence		= 10;			// ����
		_int				iHeal			= 1000;			// ȸ����
		_int				iHitRatio		= 100;			// ���߷�
		_int				iEvasion		= 1000;			// ȸ�Ƿ� 
		_int				iCritical		= 200;			// ũ��Ƽ�� Ȯ�� ������ 10�ϸ��
		_int				iCriticalDamage = 200;			// ũ��Ƽ�� ���� ������ 100�ϸ��
		_int				iStability		= 2000;			// ������
		_int				iRange			= 700;			// ��Ÿ�
		_int				iCrowdReinforce = 100;			// �������ȭ
		_int				iCrowdResist	= 100;			// ������������
	}T_UnitInfo;

	typedef struct tagMonsterInfo
	{
		_uint				iLevel;
		WCHAR				wszDisplayName[64];
		WCHAR				wszName[64];
		E_CHAR_ATKTYPE		eAtkType;
		E_CHAR_WEAPONTYPE	eWeaponType;
		E_CHAR_DFFTYPE		eDiffType;
		E_MONSTER_TYPE		eMonsterType;
		_float				fRadius;
		_vec3				vScale;

		_bool operator==(tagMonsterInfo tMonsterInfo)
		{
			if (iLevel != tMonsterInfo.iLevel)
				return FALSE;
			if (lstrcmp(wszDisplayName, tMonsterInfo.wszDisplayName))
				return FALSE;
			if (lstrcmp(wszName,tMonsterInfo.wszName))
				return FALSE;
			if (eAtkType != tMonsterInfo.eAtkType)
				return FALSE;
			if (eWeaponType != tMonsterInfo.eWeaponType)
				return FALSE;
			if (eDiffType != tMonsterInfo.eDiffType)
				return FALSE;
			if (eMonsterType != tMonsterInfo.eMonsterType)
				return FALSE;
			if (fRadius != tMonsterInfo.fRadius)
				return FALSE;
			if (vScale != tMonsterInfo.vScale)
				return FALSE;
			return TRUE;
		}
	}T_MONSTER_INFO;

	typedef struct tagCharacterInfo
	{
		_bool				bGetChar = FALSE; 
		_bool				bHide = FALSE; 

		// �ٽ� ���� (�ʼ�)
		E_CHAR_NAME			eName[(_uint)E_CHAR_LANG::LANG_END] = { E_CHAR_NAME::Name_End, E_CHAR_NAME::Name_End };
		wstring				wstrKorName = L""; 
		E_CHAR_STAR			eStar = E_CHAR_STAR::STAR_END;
		E_CHAR_COMBATTYPE	eCombatType = E_CHAR_COMBATTYPE::COMBATTYPE_END;
		E_CHAR_ROLE			eRole = E_CHAR_ROLE::ROLE_END;
		E_CHAR_POSITION		ePosition = E_CHAR_POSITION::POSITION_END;
		E_CHAR_ATKTYPE		eAtkType = E_CHAR_ATKTYPE::ATKTYPE_END;
		E_CHAR_DFFTYPE		eDffType = E_CHAR_DFFTYPE::DFFTYPE_END;
		E_CHAR_WEAPONTYPE	eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_END;
		E_CHAR_MAPBUFF		eInCityBuff = E_CHAR_MAPBUFF::MAPBUFF_END;
		E_CHAR_MAPBUFF		eOutDoorBuff = E_CHAR_MAPBUFF::MAPBUFF_END;
		E_CHAR_MAPBUFF		eInDoorBuff = E_CHAR_MAPBUFF::MAPBUFF_END;
		E_CHAR_SCHOOL		eSchoolType = E_CHAR_SCHOOL::SCHOOL_END;
		E_CHAR_GRADE		eGrade = E_CHAR_GRADE::GRADE_END;

		// �ΰ� ���� 
		wstring				wstrCircle = L"";
		wstring				wstrGeneralInfo = L"";
		_uint				iAge = 0;
		wstring				wstrBirthDay = L"";
		_uint				iHeight = 0;
		wstring				wstrHobby = L"";

		// ��ų ����
		wstring				wstrExSkillName = L"";
		wstring				wstrNormalSkillName = L"";
		wstring				wstrReinforceSkillName = L"";
		wstring				wstrSubSkillName = L"";

		E_CHAR_SKILL_LV		eSkillLvArr[(_uint)E_SkillType::SkillType_End] 
			= { E_CHAR_SKILL_LV::SKILL_LV_1, E_CHAR_SKILL_LV::SKILL_LV_1, E_CHAR_SKILL_LV::SKILL_LV_1, E_CHAR_SKILL_LV::SKILL_LV_1 };
		
		// ��ų �̹��� ���ϸ� 
		wstring				wstrExSkillImageFile = L"";
		wstring				wstrNormalSkillImageFile = L"";
		wstring				wstrReinforceSkillImageFile = L"";
		wstring				wstrSubSkillImageFile = L"";

		wstring				wstrExSkillDesc = L""; 
		wstring				wstrNormalSkillDesc = L""; 
		wstring				wstrReinforceSkillDesc = L""; 
		wstring				wstrSubSkillDesc = L""; 

		// ��ų ��ġ 
		_float				fExSkill = 0.f;
		_float				fNormalSkill = 0.f;
		_float				fReinforceSkill = 0.f;
		_float				fSubSkill = 0.f;
		_uint				iExSkillCost = 0;

		// ��ų ������ 
		_float				fExSkill_Increase = 0.f;
		_float				fNormalSkill_Increase = 0.f;
		_float				fReinforceSkill_Increase = 0.f;
		_float				fSubSkill_Increase = 0.f;

		// ���� �⺻ ���� 
		tagUnitInfo			tUnitInfo;

		// �÷��̾� ĳ���͸� ������ ���� 
		_uint				iCostHeal = 0;			// �ڽ�Ʈ ġ���� 
		_uint				iIncLevel_HP = 0;		// ������ ���� ü�� 
		_uint				iIncLevel_ATK = 0;
		_uint				iIncLevel_DEF = 0;
		_uint				iIncLevel_HEAL = 0;

		// ����, ����ġ ��� 
		_uint				iLevel = 1;
		_uint				iExperience = 0;
		_uint				iMaxExperience = iLevel * CharExpInc;

		// ������ ���� �߰�

	}T_CharInfo;

	typedef struct tagDialogInfo
	{
		tagDialogInfo() {}

		_int					iIndex = 0;
		_int					iBranch = 0;										// ������ ������ �бⰡ �־��� �� �б� 
		wstring					wstrCharName = L"";
		wstring					wstrCharCircle = L"";
		wstring					wstrTextDialog = L"";
		wstring					wstrDialogFontPos = L"";
		_uint					iBranchOptionCount = 0;								// ������ �ɼ� ����
		_uint					iGotoBranchIndex = 0;								// �ش� �ε����� ���� 
		E_CharacterMouseTag		eFaceTag = E_CharacterMouseTag::MOUSE_END;			// ǥ��
		_uint					iSpecialEventIndex = 0;								// ����� �̺�Ʈ(�ѹ��� ��� ����� �������� �̺�Ʈ ���� ���� �Ľ�)
		wstring					wstrBackgroundFile = L"";							// ��׶��� ���� �̸� 
		wstring					wstrScreenEvent = L"";								// Screen Event 
		wstring					wstrBgmFile = L"";									// Bgm ���� �̸� 
		wstring					wstrSfxFile = L"";									// Sfx ���� �̸� 

		wstring					wstrBranchText_1 = L"";								// ������ �ؽ�Ʈ 
		wstring					wstrBranchText_2 = L"";								// ������ �ؽ�Ʈ 
		wstring					wstrBranchText_3 = L"";								// ������ �ؽ�Ʈ 

		wstring					wstrEpisodeCount = L""; 
		wstring					wstrEpisodeName = L""; 
		wstring					wstrEpisodeNext = L""; 
		wstring					wstrStorySummary = L""; 

	}T_DialogInfo;

	typedef struct tagDialogCharEvent
	{
		_int					iIndex = 0; 
		wstring					wstrCharName = L""; 
		wstring					wstrCharEvent = L""; 
		wstring					wstrCharPosChange = L""; 
		wstring					wstrUniqueness = L""; 

	}T_DialogCharInfo;

	typedef struct tagAnimationTrigger
	{
		_int iTrgIndex = TRIGGER_DEFAULT;
		_int iTriggerFrame = 50;
		_int iMaxFrame = 100;
		_float fActiveDeltaTime = 0.5f;
		E_Trigger eTrigger = E_Trigger::Trigger_End;
	}T_AnimTrg;

	typedef struct tagAnimationActionSystem		// �긦 ���� Value, Key�� wstring����
	{
		tagAnimationActionSystem(wstring actionKey)
			: wstrActionKey(actionKey)
		{

		}
		wstring wstrActionKey;
		queue<_int> qAnimation;
	}T_AnimActionSys;

	typedef struct tagItemInfo
	{
		tagItemInfo() { }
		tagItemInfo(E_ItemTag _eItemTag, wstring _wPath, E_Money _eBuyType, E_ItemFilter _eItemFilter,
					wstring _wName, wstring _wDescription, _uint _iCount, _int _iItemEffect)
			: eItemTag(_eItemTag), eBuyType(_eBuyType), eItemFilter(_eItemFilter)
			, wstrName(_wName), wstrDescription(_wDescription), iCount(_iCount), iItemEffect(_iItemEffect)
			, wstrImagePath(_wPath)
		{
		}

		wstring			wstrName;
		wstring			wstrDescription;
		wstring			wstrImagePath;
		E_ItemTag		eItemTag = E_ItemTag::ItemTag_End;
		E_Money			eBuyType = E_Money::Money_End;
		E_ItemFilter	eItemFilter = E_ItemFilter::Filter_End;
		_uint			iCount = 0;
		_int			iMaxCount = 9999;
		_int			iItemEffect = 1;
	}T_ItemInfo;

	typedef struct tagEquipmentInfo
	{
		tagEquipmentInfo() {}
		tagEquipmentInfo(E_EquipTag _eEquipTag, E_EquipType _eEquipType, E_EquipTier _eEquipTier,
						 wstring _wstrName, wstring _wstrDescription, _uint _iCount, _float _fEquipEffect)
			: eEquipTag(_eEquipTag), eEquipType(_eEquipType), eEquipTier(_eEquipTier) 
			, wstrName(_wstrName), wstrDescription(_wstrDescription), iCount(_iCount), fEquipEffect(_fEquipEffect)
		{
		}

		E_EquipTag		eEquipTag = E_EquipTag::Equip_End;
		E_EquipType		eEquipType = E_EquipType::EquipType_End;
		E_EquipTier		eEquipTier = E_EquipTier::Tier_End; 

		wstring			wstrName;
		wstring			wstrDescription; 
		_uint			iCount = 0;
		_float			fEquipEffect = 1.f;
	}T_EquipInfo;

	typedef struct tagFurnitureInfo
	{
		tagFurnitureInfo() {}
		tagFurnitureInfo(E_FurnitureTag _eFurnitureTag, E_FurnitureStar _eFurnitureStar,
			wstring _wstrName, wstring _wstrDescription, _uint _iCount, _int _iAmenity, 
			wstring _TexPath, wstring _MeshPath, wstring _MeshName,
			wstring _Filter, wstring _FilterTag)
			: eFurnitureTag(_eFurnitureTag), eFurnitureStar(_eFurnitureStar)
			, wstrName(_wstrName), wstrDescription(_wstrDescription),iCount(_iCount), iAmenity(_iAmenity)
			, wstrTexturePath(_TexPath), wstrMeshPath(_MeshPath), wstrMeshName(_MeshName)
			, wstrFilter(_Filter), wstrFilterTag(_FilterTag)
		{
		}

		E_FurnitureTag		eFurnitureTag = E_FurnitureTag::Furniture_End;
		E_FurnitureStar		eFurnitureStar = E_FurnitureStar::STAR_END;

		wstring			wstrName;			//�̸�
		wstring			wstrTexturePath;	//�ؽ��� ���
		wstring			wstrMeshPath;		//�޽� ���
		wstring			wstrMeshName;		//�޽� �̸�
		wstring			wstrDescription;	//����
		_uint			iCount = 0;			//����
		_int			iAmenity = 0;		//������

		wstring			wstrFilter;			//����
		wstring			wstrFilterTag;		//���� �±�

	}T_FurnitureInfo;

	typedef struct tagEffectBase
	{
		wstring	textPath = L"None";
		_bool	bIsMesh = FALSE;
		wstring	meshPath = L"None";
		_bool	bIsSprite = FALSE;
		_int	iRow = 1;
		_int	iColumn = 1;

		wstring	filterPath_1 = L"None";
		wstring	filterPath_2 = L"None";
		wstring	filterPath_3 = L"None";
	}T_EffectBase;

	typedef struct tagEffectTexture
	{
		wstring	textPath = L"None";
		_int	iTextureType = 0;

		_bool	bIsMesh = FALSE;
		wstring	meshPath = L"None";

		_bool	bIsSprite = FALSE;
		_int	iRow = 1;
		_int	iColumn = 1;

	}T_EffectTexture;

	typedef struct tagEffectInfo
	{
		wstring	textPath = L"None";

		E_EffectRenderType eEffectRenderType = E_EffectRenderType::Buffer;
		wstring	meshPath = L"None";

		E_EffectType eEffectType = E_EffectType::Once;
		_float	fSpeedCount = 1.f;

		_float	fDelayTime = 0.f;
		_float	fLifeTime = 1.f;

		_int	iPass = 0;
		_float	fU_Start = 0.f;
		_float	fU_End = 0.f;
		_float	fV_Start = 0.f;
		_float	fV_End = 0.f;

		_vec3	vPos_Start = _vec3(0.f, 0.f, 0.f);
		_vec3	vPos_End = vPos_Start;
		_vec3	vRot_Start = _vec3(0.f, 0.f, 0.f);
		_vec3	vRot_End = vRot_Start;
		_vec3	vScale_Start = _vec3(1.f, 1.f, 1.f);
		_vec3	vScale_End = vScale_Start;

		_vec4	vColor_Start = _vec4(1.f, 1.f, 1.f, 1.f);
		_vec4	vColor_End = vScale_Start;

		_bool	bIsSprite = FALSE;
		_int	iRow = 1;
		_int	iColumn = 1;
		
		wstring	filterPath_1 = L"None";
		wstring	filterPath_2 = L"None";
		wstring	filterPath_3 = L"None";

		_bool	bBlackAlpha = FALSE;
		_bool	bBlackWhite = FALSE;

		_int	iTemp[10] = { };
		_bool	bTemp[10] = { };
		_float	fTemp[20] = { 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f};
		// 0~3 filter_1 �� �ɼ�

	}T_EffectInfo;

	typedef struct tagAstarNode
	{
		tagAstarNode() { }
		tagAstarNode(_vec3 vPos)
			: vPosition(vPos)
		{

		}

		_vec3 vPosition;
	}T_AstarNode;


	//	Amount �� �̵� �Ÿ�, fTime �� �̵� �ð�, eEase �̵� � �׷���
	typedef struct tagUiMoving
	{
		tagUiMoving(_vec2 _vAmount, _float _fTotalTime, E_EASE _eEase)
			: vAmount(_vAmount), fTotalTime(_fTotalTime), eEase(_eEase)
		{
		}
		_vec2	vStart		= _vec2(0.f, 0.f);
		_vec2	vAmount		= _vec2(0.f, 0.f);
		_float	fTime		= 0.f;
		_float	fTotalTime	= 1.f;
		E_EASE	eEase		= E_EASE::None;
	}T_UiMoving;

	typedef struct tagTileNode
	{
		_int iTileIndex;
		//���� ��忡�� �����
		_float fG;
		//���� ��忡�� ������
		_float fH;
		//F = G + H
		_float fF;

		tagTileNode* pParentTile;
	}T_TILENODE;

	typedef struct tagShopItemInfo
	{
		_bool	bAvailable	= TRUE;					// �������� �Ǹ� ���� ����

		_int	iID			= 0;					// �������� ID �ε���, ��ư�� �Բ� ���ɰ�

		_int	iRare		= 1;					// �������� ���
		
		E_Money	eBuyType	= E_Money::ũ����;	// �������� ���� ��ȭ ����
		_int	iPrice		= 1;					// �������� �Ǹ� ����

		_int	iAmount		= 1;					// �ѹ��� �Ǹ��ϴ� ������ �Ǹ� ����
		_int	iMax		= 1;					// �������� �Ǹ� ���� ����


	}T_ShopItemInfo;

	typedef struct tagRenderOption
	{
		_bool	bHDR = TRUE;
		_float  fLuminance = 0.068f;
		_float  fMiddleGray = 0.18f;
		_float  fWhiteCutoff = 0.9f;

		_bool	bBloom = TRUE;
		_float  fBrightFilter = 1.f;
		
	}T_RenderOption;

	typedef struct tagCustomFog
	{
		// ���� ������ ������
		_vec3		vPos = _vec3(0.f, 0.f, 0.f);
		
		// ������ ������ ���� ���� (����)
		_float		fView_Under = 0.f;
		// ���� ������ ���� ���� (����)
		_float		fView_Over = 0.f;

		// ������ �������� ��ġ
		_float		fDistance_Start = 1.f;
		// �������� ����. �⺻�� ������ ����
		_vec4		vColor_Start = _vec4(1.f, 0.f, 0.f, 0.f);

		// ������ ������ ��ġ
		_float		fDistance_End = 2.f;
		// ������ ����. �⺻�� �Ķ�
		_vec4		vColor_End = _vec4(0.f, 0.f, 1.f, 0.f);

	}T_CustomFog;

	typedef struct tagGlowOption
	{
		float		fBaseGlow	= 0.f;

		float		fTexGlowR	= 0.f;
		float		fTexGlowG	= 0.f;
		float		fTexGlowB	= 1.f;

		float		fGlowPower	= 1.f;
		
		float		fGlow_U_Min = 0.f;
		float		fGlow_U_Max = 1.f;

		float		fGlow_V_Min = 0.f;
		float		fGlow_V_Max = 1.f;

		_vec3		vGlowColor = _vec3(1.f, 1.f, 1.f);
		
	}T_GlowOption;

	typedef struct tagRankingInfo
	{
		string strPlayFabID;
		wstring wstrNickname;
		time_t tLastLogin;
		_int iRank;
		_int iBossScore[10] = { 0, };
		_int iAccountLevel;
		_int iLobbyChar;
		_int iSquadChar_Enum[6];
		_int iSquadChar_Level[6];
		_int iSquadChar_Star[6];
	}T_PlayfabInfo;

	typedef struct tagSkillGuideOption
	{
		_vec3 vPosition;
		_vec3 vRotation;
		_float fMinDistance;
		_float fMaxDistance;
		_float fAngle;
		_float fDelay;
		_float fWidth;
	}T_GUIDE_OPTION;

	typedef struct tagGodLight
	{
		_vec3	vPosition = _vec3(0.f, 0.f, 0.f);
		_vec4	vColor = _vec4(1.f, 1.f, 1.f, 1.f);
		_float	fSize = 1.f;

		_bool	bSun = FALSE;

	}T_GodLight;

	typedef struct tagCombatStatus
	{
		tagCombatStatus(E_Combat_Status _eStatus, _float _fLeftTime)
			: eStatus(_eStatus), fLeftTime(_fLeftTime)
		{

		};

		E_Combat_Status		eStatus;
		_float				fLeftTime = 3.f;
	}T_CombatStatus;

	typedef struct tagViewCut
	{
		tagViewCut() 
		{
			fTime = 0.f;
			fTimeMax = 0.f;
			fDelay = 0.f;

			fSpeed = 1.f;
			fDis = 0.2f;

			D3DXMatrixIdentity(&matView_ViewCut);
			D3DXMatrixIdentity(&matProg_ViewCut);
		}
		
		// ���ۺ��� ��������� �� ���ӽð��Դϴ�.
		_float		fTimeMax	= 0.f;

		// life�� ����ϱ� ���� �ð��Դϴ�.
		_float		fTime = 0.f;

		// ���� �ð��� ������ �����ִ� �ð��Դϴ�.
		_float		fDelay = 0.f;

		// �ӵ��� ���� �ð��� �ݺ���մϴ�
		_float		fSpeed	= 1.f;

		// �� ���� �Ʒ��� ���� ���� Ƣ��� ����
		_float		fDis = 0.2f;

		// ����Ǵ� �� ��Ʈ����
		_matrix		matView_ViewCut;
		// ����Ǵ� ���� ��Ʈ����
		_matrix		matProg_ViewCut;

	}T_ViewCut;

	typedef struct tagUnitSoundFileNames
	{
		tagUnitSoundFileNames()
		{
			pairShoot.first = SoundEmpty;
			pairShoot.second = 0.5f;

			pairReload.first	= SoundEmpty;
			pairReload.second	= 0.5f;
		}

		pair<wchar_t*, _float> pairShoot;
		pair<wchar_t*, _float> pairReload;
	}T_UnitSound;

	typedef struct tagBuffDebuff
	{
		tagBuffDebuff(E_BUFFDEBUFF_TYPE _eType, _float _fEffect, const _float& _fLeftTime)
			: eType(_eType), fEffect(_fEffect), fLeftTime(_fLeftTime) {}

		E_BUFFDEBUFF_TYPE	eType = E_BUFFDEBUFF_TYPE::BUFFDEBUFF_END;
		_float	fEffect = 0.f; 
		_float	fLeftTime = 0.f; 

	}T_BuffDebuff;
}
#endif // __Engine_Struct_H__

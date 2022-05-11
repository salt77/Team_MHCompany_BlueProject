// Tool_Particle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Particle.h"
#include "afxdialogex.h"


// CTool_Particle 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTool_Particle, CDialogEx)

CTool_Particle::CTool_Particle(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_PARTICLE, pParent)
	, m_strSampleAddMinPosX(_T(""))
	, m_strSampleAddMinPosY(_T(""))
	, m_strSampleAddMinPosZ(_T(""))
	, m_strSampleAddMaxPosX(_T(""))
	, m_strSampleAddMaxPosY(_T(""))
	, m_strSampleAddMaxPosZ(_T(""))
	, m_strSampleAddMinRotX(_T(""))
	, m_strSampleAddMinRotY(_T(""))
	, m_strSampleAddMinRotZ(_T(""))
	, m_strSampleAddMaxRotX(_T(""))
	, m_strSampleAddMaxRotY(_T(""))
	, m_strSampleAddMaxRotZ(_T(""))
	, m_strSampleScaleX(_T(""))
	, m_strSampleScaleY(_T(""))
	, m_strSampleScaleZ(_T(""))
	, m_strSampleAddMinScaleX(_T(""))
	, m_strSampleAddMinScaleY(_T(""))
	, m_strSampleAddMinScaleZ(_T(""))
	, m_strSampleColorA(_T(""))
	, m_strSampleAddMinColorA(_T(""))
	, m_strSampleLifeTime(_T(""))
	, m_strSpawnerPosX(_T(""))
	, m_strSpawnerPosY(_T(""))
	, m_strSpawnerPosZ(_T(""))
	, m_strSpawnerRotX(_T(""))
	, m_strSpawnerRotY(_T(""))
	, m_strSpawnerRotZ(_T(""))
	, m_strSpawnerSpawnMinPosX(_T(""))
	, m_strSpawnerSpawnMinPosY(_T(""))
	, m_strSpawnerSpawnMinPosZ(_T(""))
	, m_strSpawnerSpawnMaxPosX(_T(""))
	, m_strSpawnerSpawnMaxPosY(_T(""))
	, m_strSpawnerSpawnMaxPosZ(_T(""))
	, m_strSpawnerLifeTime(_T(""))
	, m_strSpawnerEntranceTime(_T(""))
	, m_strParticleObjectLifeTime(_T(""))
	, m_strSpawnerAddMinPosX(_T(""))
	, m_strSpawnerAddMinPosY(_T(""))
	, m_strSpawnerAddMinPosZ(_T(""))
	, m_strSpawnerAddMaxPosX(_T(""))
	, m_strSpawnerAddMaxPosY(_T(""))
	, m_strSpawnerAddMaxPosZ(_T(""))
	, m_strSpawnerAddMinRotX(_T(""))
	, m_strSpawnerAddMinRotY(_T(""))
	, m_strSpawnerAddMinRotZ(_T(""))
	, m_strSpawnerAddMaxRotX(_T(""))
	, m_strSpawnerAddMaxRotY(_T(""))
	, m_strSpawnerAddMaxRotZ(_T(""))
	, m_strSpawnerSpawnInterval(_T(""))
	, m_strSpawnerSampleIndex(_T(""))
	, m_strSampleTexIndex(_T(""))
	, m_strSampleGravity(_T(""))
	, m_strSampleMaxGravity(_T(""))
	, m_bSampleBillBoard(FALSE)
	, m_bSpawnerMultiSpawn(FALSE)
	, m_strSpawnerMultiStartPosX(_T(""))
	, m_strSpawnerMultiStartPosY(_T(""))
	, m_strSpawnerMultiStartPosZ(_T(""))
	, m_strSpawnerMultiStartRotX(_T(""))
	, m_strSpawnerMultiStartRotY(_T(""))
	, m_strSpawnerMultiStartRotZ(_T(""))
	, m_strSpawnerMultiAddPosX(_T(""))
	, m_strSpawnerMultiAddPosY(_T(""))
	, m_strSpawnerMultiAddPosZ(_T(""))
	, m_strSpawnerMultiAddRotX(_T(""))
	, m_strSpawnerMultiAddRotY(_T(""))
	, m_strSpawnerMultiAddRotZ(_T(""))
	, m_strSpawnerMultiCount(_T(""))
	, m_strParticleObjectScale(_T(""))
	, m_strParticleObjectRotX(_T(""))
	, m_strParticleObjectRotY(_T(""))
	, m_strParticleObjectRotZ(_T(""))
	, m_bIndependent(FALSE)
	, m_bBlackBackGroundImage(FALSE)
	, m_strSpawnMinRotX(_T(""))
	, m_strSpawnMinRotY(_T(""))
	, m_strSpawnMinRotZ(_T(""))
	, m_strSpawnMaxRotX(_T(""))
	, m_strSpawnMaxRotY(_T(""))
	, m_strSpawnMaxRotZ(_T(""))
	, m_strSampleMaxScaleX(_T(""))
	, m_strSampleMaxScaleY(_T(""))
	, m_strSampleMaxScaleZ(_T(""))
	, m_strSampleOffsetRotX(_T(""))
	, m_strSampleOffsetRotY(_T(""))
	, m_strSampleOffsetRotZ(_T(""))
	, m_strBillBoardZRot(_T(""))
	, m_strSampleMaxAlpha(_T(""))
	, m_bUseCross(FALSE)
	, m_bBillBoardLookDir(FALSE)
	, m_iRadioAlphaOption(0)
	, m_bSampleZEnable(FALSE)
	, m_bSampleBlur(FALSE)
	, m_strSpriteX(_T(""))
	, m_strSpriteY(_T(""))
	, m_strCycleNum(_T(""))
	, m_strSampleLifeTimeMax(_T(""))
{
	ZeroMemory(&m_tCopyParticleSample, sizeof(T_PARTICLE));
	ZeroMemory(&m_tCopyParticleSpawner, sizeof(T_PARTICLE_SPAWNER));
}

CTool_Particle::CTool_Particle(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_PARTICLE, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_Particle::~CTool_Particle()
{
}

void CTool_Particle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MW_SAMPLE_LIST, m_cListParticleSample);
	DDX_Control(pDX, IDC_MW_WAIT_SPAWNER_LIST, m_cListWaitSpawner);
	DDX_Control(pDX, IDC_MW_TEXTURE_LIST, m_cListTexture);
	DDX_Text(pDX, IDC_MW_ADD_MIN_POSX, m_strSampleAddMinPosX);
	DDX_Text(pDX, IDC_MW_ADD_MIN_POSY, m_strSampleAddMinPosY);
	DDX_Text(pDX, IDC_MW_ADD_MIN_POSZ, m_strSampleAddMinPosZ);
	DDX_Text(pDX, IDC_MW_ADD_MAX_POSX, m_strSampleAddMaxPosX);
	DDX_Text(pDX, IDC_MW_ADD_MAX_POSY, m_strSampleAddMaxPosY);
	DDX_Text(pDX, IDC_MW_ADD_MAX_POSZ, m_strSampleAddMaxPosZ);
	DDX_Text(pDX, IDC_MW_ADD_MIN_ROTX, m_strSampleAddMinRotX);
	DDX_Text(pDX, IDC_MW_ADD_MIN_ROTY, m_strSampleAddMinRotY);
	DDX_Text(pDX, IDC_MW_ADD_MIN_ROTZ, m_strSampleAddMinRotZ);
	DDX_Text(pDX, IDC_MW_ADD_MAX_ROTX, m_strSampleAddMaxRotX);
	DDX_Text(pDX, IDC_MW_ADD_MAX_ROTY, m_strSampleAddMaxRotY);
	DDX_Text(pDX, IDC_MW_ADD_MAX_ROTZ, m_strSampleAddMaxRotZ);
	DDX_Text(pDX, IDC_MW_SCALEX, m_strSampleScaleX);
	DDX_Text(pDX, IDC_MW_SCALEY, m_strSampleScaleY);
	DDX_Text(pDX, IDC_MW_SCALEZ, m_strSampleScaleZ);
	DDX_Text(pDX, IDC_MW_ADD_MIN_SCALEX, m_strSampleAddMinScaleX);
	DDX_Text(pDX, IDC_MW_ADD_MIN_SCALEY, m_strSampleAddMinScaleY);
	DDX_Text(pDX, IDC_MW_ADD_MIN_SCALEZ, m_strSampleAddMinScaleZ);
	DDX_Text(pDX, IDC_MW_EDIT_SAMPLE_A, m_strSampleColorA);
	DDX_Text(pDX, IDC_MW_EDIT_SAMPLE_ADD_MIN_COLOR_A, m_strSampleAddMinColorA);


	DDX_Text(pDX, IDC_MW_EDIT_SAMPLE_LIFE_TIME, m_strSampleLifeTime);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_POS_X, m_strSpawnerPosX);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_POS_Y, m_strSpawnerPosY);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_POS_Z, m_strSpawnerPosZ);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_ROT_X, m_strSpawnerRotX);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_ROT_Y, m_strSpawnerRotY);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_ROT_Z, m_strSpawnerRotZ);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_SPAWN_MIN_POS_X, m_strSpawnerSpawnMinPosX);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_SPAWN_MIN_POS_Y, m_strSpawnerSpawnMinPosY);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_SPAWN_MIN_POS_Z, m_strSpawnerSpawnMinPosZ);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_SPAWN_MAX_POS_X, m_strSpawnerSpawnMaxPosX);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_SPAWN_MAX_POS_Y, m_strSpawnerSpawnMaxPosY);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_SPAWN_MAX_POS_Z, m_strSpawnerSpawnMaxPosZ);
	DDX_Control(pDX, IDC_MW_PICTURE_VIEWER, m_cPicture);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_LIFE_TIME, m_strSpawnerLifeTime);
	DDX_Text(pDX, IDC_MW_EDIT_SPAWNER_ENTRANCE_TIME, m_strSpawnerEntranceTime);
	DDX_Text(pDX, IDC_MW_EDIT_PARTICLE_OBJECT_LIFE_TIME, m_strParticleObjectLifeTime);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MIN_POSX, m_strSpawnerAddMinPosX);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MIN_POS_Y, m_strSpawnerAddMinPosY);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MIN_POS_Z, m_strSpawnerAddMinPosZ);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MAX_POS_X, m_strSpawnerAddMaxPosX);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MAX_POS_Y, m_strSpawnerAddMaxPosY);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MAX_POS_Z, m_strSpawnerAddMaxPosZ);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MIN_ROT_X, m_strSpawnerAddMinRotX);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MIN_ROT_Y, m_strSpawnerAddMinRotY);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MIN_ROT_Z, m_strSpawnerAddMinRotZ);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MAX_ROT_X, m_strSpawnerAddMaxRotX);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MAX_ROT_Y, m_strSpawnerAddMaxRotY);
	DDX_Text(pDX, IDC_MW_1216_SPAWNER_ADD_MAX_ROT_Z, m_strSpawnerAddMaxRotZ);
	DDX_Text(pDX, IDC_MW_1217_EDIT_SPAWNER_SPAWN_INTERVAL, m_strSpawnerSpawnInterval);
	DDX_Text(pDX, IDC_MW_1217_EDIT_SPAWNER_SAMPLE_INDEX, m_strSpawnerSampleIndex);
	DDX_Text(pDX, IDC_MW_1217_EDIT_SAMPLE_INDEX, m_strSampleTexIndex);
	DDX_Control(pDX, IDC_MW_SAMPLE_SYSTEM_COMBO, m_cComboSampleType);
	DDX_Control(pDX, IDC_MW_COMBO_SPAWNER_SYSTEM, m_cComboSpawnerType);
	DDX_Text(pDX, IDC_MW_1219_EDIT_GRAVITY, m_strSampleGravity);
	DDX_Text(pDX, IDC_MW_1219_EDIT_MAX_GRAVITY, m_strSampleMaxGravity);
	DDX_Check(pDX, IDC_MW_1219_CHECK_SAMPLE_BILLBOARD, m_bSampleBillBoard);
	DDX_Check(pDX, IDC_MW_1220_CHECK_MULTIPLE, m_bSpawnerMultiSpawn);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_START_POS_X, m_strSpawnerMultiStartPosX);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_START_POS_Y, m_strSpawnerMultiStartPosY);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_START_POS_Z, m_strSpawnerMultiStartPosZ);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_START_ROT_X, m_strSpawnerMultiStartRotX);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_START_ROT_Y, m_strSpawnerMultiStartRotY);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_START_ROT_Z, m_strSpawnerMultiStartRotZ);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_POS_X, m_strSpawnerMultiAddPosX);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_POS_Y, m_strSpawnerMultiAddPosY);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_POS_Z, m_strSpawnerMultiAddPosZ);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_ROT_X, m_strSpawnerMultiAddRotX);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_ROT_Y, m_strSpawnerMultiAddRotY);
	DDX_Text(pDX, IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_ROT_Z, m_strSpawnerMultiAddRotZ);
	DDX_Text(pDX, IDC_MW_1220_EDIT_MULTI_COUNT, m_strSpawnerMultiCount);
	DDX_Text(pDX, IDC_MW_0107_EDIT_PARTICLE_OBJECT_SCALE, m_strParticleObjectScale);
	DDX_Control(pDX, IDC_MW_0107_COLOR_START, m_cColorStart);
	DDX_Control(pDX, IDC_MW_0107_COLOR_END, m_cColorEnd);
	DDX_Text(pDX, IDC_MW_0107_EDIT_PARTICLE_OBJECT_ROT_X, m_strParticleObjectRotX);
	DDX_Text(pDX, IDC_MW_0107_EDIT_PARTICLE_OBJECT_ROT_Y, m_strParticleObjectRotY);
	DDX_Text(pDX, IDC_MW_0107_EDIT_PARTICLE_OBJECT_ROT_Z, m_strParticleObjectRotZ);
	DDX_Check(pDX, IDC_MW_0107_CHECK_INDEPENDENT, m_bIndependent);
	DDX_Check(pDX, IDC_MW_0114_CHECK_BLACK_IMAGE, m_bBlackBackGroundImage);
	DDX_Text(pDX, IDC_MW_0119_EDIT_SPAWNER_SPAWN_MIN_ROT_X, m_strSpawnMinRotX);
	DDX_Text(pDX, IDC_MW_0119_EDIT_SPAWNER_SPAWN_MIN_ROT_Y, m_strSpawnMinRotY);
	DDX_Text(pDX, IDC_MW_0119_EDIT_SPAWNER_SPAWN_MIN_ROT_Z, m_strSpawnMinRotZ);
	DDX_Text(pDX, IDC_MW_0119_EDIT_SPAWNER_SPAWN_MAX_ROT_, m_strSpawnMaxRotX);
	DDX_Text(pDX, IDC_MW_0119_EDIT_SPAWNER_SPAWN_MAX_ROT_Y, m_strSpawnMaxRotY);
	DDX_Text(pDX, IDC_MW_0119_EDIT_SPAWNER_SPAWN_MAX_ROT_Z, m_strSpawnMaxRotZ);
	DDX_Text(pDX, IDC_MW_0119_MAX_SCALEX, m_strSampleMaxScaleX);
	DDX_Text(pDX, IDC_MW_0119_MAX_SCALEY, m_strSampleMaxScaleY);
	DDX_Text(pDX, IDC_MW_0119_MAX_SCALEZ, m_strSampleMaxScaleZ);
	DDX_Text(pDX, IDC_MW_0119_EDIT_OFFSET_ROT_X, m_strSampleOffsetRotX);
	DDX_Text(pDX, IDC_MW_0119_EDIT_OFFSET_ROT_Y, m_strSampleOffsetRotY);
	DDX_Text(pDX, IDC_MW_0119_EDIT_OFFSET_ROT_Z, m_strSampleOffsetRotZ);
	DDX_Text(pDX, IDC_MW_0119_EDIT_BILLBOARD_Z_ROT, m_strBillBoardZRot);
	DDX_Text(pDX, IDC_MW_0119_EDIT_MAX_ALPHA, m_strSampleMaxAlpha);
	DDX_Control(pDX, IDC_MW_0119_COLOR_START_MAX, m_cColorStartMax);
	DDX_Check(pDX, IDC_MW_0119_CHECK_USE_CROSS, m_bUseCross);
	DDX_Check(pDX, IDC_MW_0119_CHECK_BILLBOARD_LOOK_DIR, m_bBillBoardLookDir);
	DDX_Radio(pDX, IDC_MW_0120_RADIO_ALPHA_TRANS, m_iRadioAlphaOption);
	DDX_Check(pDX, IDC_MW_0120_CHECK_ZENABLE, m_bSampleZEnable);
	DDX_Check(pDX, IDC_MW_0120_CHECK_Blur, m_bSampleBlur);
	DDX_Text(pDX, IDC_MW_0120_EDIT_SPRITE_X, m_strSpriteX);
	DDX_Text(pDX, IDC_MW_0120_EDIT_SPRITE_Y, m_strSpriteY);
	DDX_Text(pDX, IDC_MW_0120_EDIT_SPRITE_CYCLE, m_strCycleNum);
	DDX_Text(pDX, IDC_MW_0121_EDIT_SAMPLE_LIFE_TIME_MAX, m_strSampleLifeTimeMax);
}


BEGIN_MESSAGE_MAP(CTool_Particle, CDialogEx)
	
	ON_BN_CLICKED(IDC_MW_BUTTON_ADD_SAMPLE, &CTool_Particle::OnBnClickedMwButtonAddSample)
	ON_BN_CLICKED(IDC_MW_BUTTON_DELETE_SAMPLE, &CTool_Particle::OnBnClickedMwButtonDeleteSample)
	ON_BN_CLICKED(IDC_MW_DELETE_SPAWNER, &CTool_Particle::OnBnClickedMwDeleteSpawner)
	ON_BN_CLICKED(IDC_MW_ADD_SPAWNER, &CTool_Particle::OnBnClickedMwAddSpawner)
	ON_BN_CLICKED(IDC_MW_DELETE_TEXTURE, &CTool_Particle::OnBnClickedMwDeleteTexture)
	ON_BN_CLICKED(IDC_MW_BUTTON_ADD_TEXTURE, &CTool_Particle::OnBnClickedMwButtonAddTexture)
	ON_BN_CLICKED(IDC_MW_BUTTON_SAVE_PARTICLE, &CTool_Particle::OnBnClickedMwButtonSaveParticle)
	ON_BN_CLICKED(IDC_MW_LOAD_PARTICLE, &CTool_Particle::OnBnClickedMwLoadParticle)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_MW_SAMPLE_LIST, &CTool_Particle::OnLbnSelchangeMwSampleList)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_POSX, &CTool_Particle::OnEnChangeMwAddMinPosx)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_POSY, &CTool_Particle::OnEnChangeMwAddMinPosy)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_POSZ, &CTool_Particle::OnEnChangeMwAddMinPosz)
	ON_EN_CHANGE(IDC_MW_ADD_MAX_POSX, &CTool_Particle::OnEnChangeMwAddMaxPosx)
	ON_EN_CHANGE(IDC_MW_ADD_MAX_POSY, &CTool_Particle::OnEnChangeMwAddMaxPosy)
	ON_EN_CHANGE(IDC_MW_ADD_MAX_POSZ, &CTool_Particle::OnEnChangeMwAddMaxPosz)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_ROTX, &CTool_Particle::OnEnChangeMwAddMinRotx)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_ROTY, &CTool_Particle::OnEnChangeMwAddMinRoty)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_ROTZ, &CTool_Particle::OnEnChangeMwAddMinRotz)
	ON_EN_CHANGE(IDC_MW_ADD_MAX_ROTX, &CTool_Particle::OnEnChangeMwAddMaxRotx)
	ON_EN_CHANGE(IDC_MW_ADD_MAX_ROTY, &CTool_Particle::OnEnChangeMwAddMaxRoty)
	ON_EN_CHANGE(IDC_MW_ADD_MAX_ROTZ, &CTool_Particle::OnEnChangeMwAddMaxRotz)
	ON_EN_CHANGE(IDC_MW_SCALEX, &CTool_Particle::OnEnChangeMwScalex)
	ON_EN_CHANGE(IDC_MW_SCALEY, &CTool_Particle::OnEnChangeMwScaley)
	ON_EN_CHANGE(IDC_MW_SCALEZ, &CTool_Particle::OnEnChangeMwScalez)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_SCALEX, &CTool_Particle::OnEnChangeMwAddMinScalex)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_SCALEY, &CTool_Particle::OnEnChangeMwAddMinScaley)
	ON_EN_CHANGE(IDC_MW_ADD_MIN_SCALEZ, &CTool_Particle::OnEnChangeMwAddMinScalez)
	ON_EN_CHANGE(IDC_MW_EDIT_SAMPLE_A, &CTool_Particle::OnEnChangeMwEditSampleA)
	ON_EN_CHANGE(IDC_MW_EDIT_SAMPLE_ADD_MIN_COLOR_A, &CTool_Particle::OnEnChangeMwEditSampleAddMinColorA)
	ON_EN_CHANGE(IDC_MW_EDIT_SAMPLE_LIFE_TIME, &CTool_Particle::OnEnChangeMwEditSampleLifeTime)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_POS_Z, &CTool_Particle::OnEnChangeMwEditSpawnerPosZ)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_POS_Y, &CTool_Particle::OnEnChangeMwEditSpawnerPosY)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_POS_X, &CTool_Particle::OnEnChangeMwEditSpawnerPosX)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_ROT_X, &CTool_Particle::OnEnChangeMwEditSpawnerRotX)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_ROT_Y, &CTool_Particle::OnEnChangeMwEditSpawnerRotY)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_ROT_Z, &CTool_Particle::OnEnChangeMwEditSpawnerRotZ)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_SPAWN_MIN_POS_X, &CTool_Particle::OnEnChangeMwEditSpawnerSpawnMinPosX)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_SPAWN_MIN_POS_Y, &CTool_Particle::OnEnChangeMwEditSpawnerSpawnMinPosY)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_SPAWN_MIN_POS_Z, &CTool_Particle::OnEnChangeMwEditSpawnerSpawnMinPosZ)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_SPAWN_MAX_POS_X, &CTool_Particle::OnEnChangeMwEditSpawnerSpawnMaxPosX)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_SPAWN_MAX_POS_Y, &CTool_Particle::OnEnChangeMwEditSpawnerSpawnMaxPosY)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_SPAWN_MAX_POS_Z, &CTool_Particle::OnEnChangeMwEditSpawnerSpawnMaxPosZ)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_LIFE_TIME, &CTool_Particle::OnEnChangeMwEditSpawnerLifeTime)
	ON_EN_CHANGE(IDC_MW_EDIT_SPAWNER_ENTRANCE_TIME, &CTool_Particle::OnEnChangeMwEditSpawnerEntranceTime)
	ON_EN_CHANGE(IDC_MW_EDIT_PARTICLE_OBJECT_LIFE_TIME, &CTool_Particle::OnEnChangeMwEditParticleObjectLifeTime)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MIN_POSX, &CTool_Particle::OnEnChangeMw1216SpawnerAddMinPosx)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MIN_POS_Y, &CTool_Particle::OnEnChangeMw1216SpawnerAddMinPosY)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MIN_POS_Z, &CTool_Particle::OnEnChangeMw1216SpawnerAddMinPosZ)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MAX_POS_X, &CTool_Particle::OnEnChangeMw1216SpawnerAddMaxPosX)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MAX_POS_Y, &CTool_Particle::OnEnChangeMw1216SpawnerAddMaxPosY)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MAX_POS_Z, &CTool_Particle::OnEnChangeMw1216SpawnerAddMaxPosZ)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MIN_ROT_X, &CTool_Particle::OnEnChangeMw1216SpawnerAddMinRotX)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MIN_ROT_Y, &CTool_Particle::OnEnChangeMw1216SpawnerAddMinRotY)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MIN_ROT_Z, &CTool_Particle::OnEnChangeMw1216SpawnerAddMinRotZ)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MAX_ROT_X, &CTool_Particle::OnEnChangeMw1216SpawnerAddMaxRotX)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MAX_ROT_Y, &CTool_Particle::OnEnChangeMw1216SpawnerAddMaxRotY)
	ON_EN_CHANGE(IDC_MW_1216_SPAWNER_ADD_MAX_ROT_Z, &CTool_Particle::OnEnChangeMw1216SpawnerAddMaxRotZ)
	ON_LBN_SELCHANGE(IDC_MW_WAIT_SPAWNER_LIST, &CTool_Particle::OnLbnSelchangeMwWaitSpawnerList)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_MW_1217_EDIT_SPAWNER_SPAWN_INTERVAL, &CTool_Particle::OnEnChangeMw1217EditSpawnerSpawnInterval)
	ON_EN_CHANGE(IDC_MW_1217_EDIT_SPAWNER_SAMPLE_INDEX, &CTool_Particle::OnEnChangeMw1217EditSpawnerSampleIndex)
	ON_LBN_SELCHANGE(IDC_MW_TEXTURE_LIST, &CTool_Particle::OnLbnSelchangeMwTextureList)
	ON_EN_CHANGE(IDC_MW_1217_EDIT_SAMPLE_INDEX, &CTool_Particle::OnEnChangeMw1217EditSampleIndex)
	ON_CBN_SELCHANGE(IDC_MW_COMBO_SPAWNER_SYSTEM, &CTool_Particle::OnCbnSelchangeMwComboSpawnerSystem)
	ON_CBN_SELCHANGE(IDC_MW_SAMPLE_SYSTEM_COMBO, &CTool_Particle::OnCbnSelchangeMwSampleSystemCombo)
	ON_EN_CHANGE(IDC_MW_1219_EDIT_GRAVITY, &CTool_Particle::OnEnChangeMw1219EditGravity)
	ON_EN_CHANGE(IDC_MW_1219_EDIT_MAX_GRAVITY, &CTool_Particle::OnEnChangeMw1219EditMaxGravity)
	ON_BN_CLICKED(IDC_MW_1219_CHECK_SAMPLE_BILLBOARD, &CTool_Particle::OnBnClickedMw1219CheckSampleBillboard)
	ON_BN_CLICKED(IDC_MW_1220_CHECK_MULTIPLE, &CTool_Particle::OnBnClickedMw1220CehckMultiple)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_MULTI_COUNT, &CTool_Particle::OnEnChangeMw1220EditMultiCount)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_START_POS_X, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartPosX)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_START_POS_Y, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartPosY)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_START_POS_Z, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartPosZ)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_START_ROT_X, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartRotX)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_START_ROT_Y, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartRotY)
	ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_START_ROT_Z, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartRotZ)
		ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_POS_X, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddPosX)
		ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_POS_Y, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddPosY)
		ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_POS_Z, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddPosZ)
		ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_ROT_X, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddRotX)
		ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_ROT_Y, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddRotY)
		ON_EN_CHANGE(IDC_MW_1220_EDIT_SPAWNER_MULTI_ADD_ROT_Z, &CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddRotZ)
		ON_EN_CHANGE(IDC_MW_0107_EDIT_PARTICLE_OBJECT_SCALE, &CTool_Particle::OnEnChangeMw0107EditParticleObjectScale)
		ON_BN_CLICKED(IDC_MW_0107_BUTTON_SPAWNER_COPY, &CTool_Particle::OnBnClickedMw0107ButtonSpawnerCopy)
		ON_BN_CLICKED(IDC_MW_0107_PASTE_SPAWNER, &CTool_Particle::OnBnClickedMw0107PasteSpawner)
		ON_BN_CLICKED(IDC_MW_0107_BUTTON_COPY_SAMPLE, &CTool_Particle::OnBnClickedMw0107ButtonCopySample)
		ON_BN_CLICKED(IDC_MW_0107_BUTTON_PASTE_SAMPLE, &CTool_Particle::OnBnClickedMw0107ButtonPasteSample)
	ON_BN_CLICKED(IDC_MW_0107_COLOR_START, &CTool_Particle::OnBnClickedMw0107ColorStart)
	ON_BN_CLICKED(IDC_MW_0107_COLOR_END, &CTool_Particle::OnBnClickedMw0107ColorEnd)
	ON_EN_CHANGE(IDC_MW_0107_EDIT_PARTICLE_OBJECT_ROT_X, &CTool_Particle::OnEnChangeMw0107EditParticleObjectRotX)
	ON_EN_CHANGE(IDC_MW_0107_EDIT_PARTICLE_OBJECT_ROT_Y, &CTool_Particle::OnEnChangeMw0107EditParticleObjectRotY)
	ON_EN_CHANGE(IDC_MW_0107_EDIT_PARTICLE_OBJECT_ROT_Z, &CTool_Particle::OnEnChangeMw0107EditParticleObjectRotZ)
	ON_BN_CLICKED(IDC_MW_0107_CHECK_INDEPENDENT, &CTool_Particle::OnBnClickedMw0107CheckIndependent)
	ON_BN_CLICKED(IDC_MW_0114_CHECK_BLACK_IMAGE, &CTool_Particle::OnBnClickedMw0114CheckBlackImage)
	ON_EN_CHANGE(IDC_MW_0119_EDIT_SPAWNER_SPAWN_MIN_ROT_X, &CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMinRotX)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_SPAWNER_SPAWN_MIN_ROT_Y, &CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMinRotY)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_SPAWNER_SPAWN_MIN_ROT_Z, &CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMinRotZ)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_SPAWNER_SPAWN_MAX_ROT_, &CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMaxRot)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_SPAWNER_SPAWN_MAX_ROT_Y, &CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMaxRotY)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_SPAWNER_SPAWN_MAX_ROT_Z, &CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMaxRotZ)
		ON_EN_CHANGE(IDC_MW_0119_MAX_SCALEX, &CTool_Particle::OnEnChangeMw0119MaxScalex)
		ON_EN_CHANGE(IDC_MW_0119_MAX_SCALEY, &CTool_Particle::OnEnChangeMw0119MaxScaley)
		ON_EN_CHANGE(IDC_MW_0119_MAX_SCALEZ, &CTool_Particle::OnEnChangeMw0119MaxScalez)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_OFFSET_ROT_X, &CTool_Particle::OnEnChangeMw0119EditOffsetRotX)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_OFFSET_ROT_Y, &CTool_Particle::OnEnChangeMw0119EditOffsetRotY)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_OFFSET_ROT_Z, &CTool_Particle::OnEnChangeMw0119EditOffsetRotZ)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_BILLBOARD_Z_ROT, &CTool_Particle::OnEnChangeMw0119EditBillboardZRot)
		ON_EN_CHANGE(IDC_MW_0119_EDIT_MAX_ALPHA, &CTool_Particle::OnEnChangeMw0119EditMaxAlpha)
		ON_BN_CLICKED(IDC_MW_0119_COLOR_START_MAX, &CTool_Particle::OnBnClickedMw0119ColorStartMax)
		ON_BN_CLICKED(IDC_MW_0119_CHECK_USE_CROSS, &CTool_Particle::OnBnClickedMw0119CheckUseCross)
		ON_BN_CLICKED(IDC_MW_0119_CHECK_BILLBOARD_LOOK_DIR, &CTool_Particle::OnBnClickedMw0119CheckBillboardLookDir)
		ON_BN_CLICKED(IDC_MW_0120_RADIO_ALPHA_TRANS, &CTool_Particle::OnBnClickedMw0120RadioAlphaTrans)
		ON_BN_CLICKED(IDC_MW_0120_RADIO_ALPHA_BLEND, &CTool_Particle::OnBnClickedMw0120RadioAlphaBlend)
		ON_BN_CLICKED(IDC_MW_0120_RADIO_NON_ALPHA, &CTool_Particle::OnBnClickedMw0120RadioNonAlpha)
		ON_BN_CLICKED(IDC_MW_0120_CHECK_Blur, &CTool_Particle::OnBnClickedMw0120CheckBlur)
		ON_BN_CLICKED(IDC_MW_0120_CHECK_ZENABLE, &CTool_Particle::OnBnClickedMw0120CheckZenable)
		ON_EN_CHANGE(IDC_MW_0120_EDIT_SPRITE_X, &CTool_Particle::OnEnChangeMw0120EditSpriteX)
		ON_EN_CHANGE(IDC_MW_0120_EDIT_SPRITE_Y, &CTool_Particle::OnEnChangeMw0120EditSpriteY)
		ON_EN_CHANGE(IDC_MW_0120_EDIT_SPRITE_CYCLE, &CTool_Particle::OnEnChangeMw0120EditSpriteCycle)
		ON_EN_CHANGE(IDC_MW_0121_EDIT_SAMPLE_LIFE_TIME_MAX, &CTool_Particle::OnEnChangeMw0121EditSampleLifeTimeMax)
		END_MESSAGE_MAP()


// CTool_Particle 메시지 처리기입니다.



//파티클 샘플(깡통)을 추가하는 함수
void CTool_Particle::OnBnClickedMwButtonAddSample()
{
	T_PARTICLE tParticleSample;

	if (!m_bPasteSample)
	{
		ZeroMemory(&tParticleSample, sizeof(tParticleSample));
		tParticleSample.vScale = _vec3(1.f, 1.f, 1.f);
		tParticleSample.vScaleMax = _vec3(1.f, 1.f, 1.f);
		tParticleSample.vEndScale = _vec3(1.f, 1.f, 1.f);
		tParticleSample.vStartColor = _vec4(1.f, 1.f, 1.f, 1.f);
		tParticleSample.vStartColorMax = _vec4(1.f, 1.f, 1.f, 1.f);
		tParticleSample.vEndColor = _vec4(1.f, 1.f, 1.f, 1.f);
		tParticleSample.fLifeTime = 1.f;
		tParticleSample.bZEnable = TRUE;
		tParticleSample.iSpriteX = 1;
		tParticleSample.iSpriteY = 1;
		tParticleSample.fCycle = 1.f;
		tParticleSample.fMaxLifeTime = 1.f;
	}
	else
	{
		tParticleSample = m_tCopyParticleSample;
	}
	

	WCHAR wszSampleName[32];
	swprintf_s(wszSampleName, L"Particle Sample %d", m_pParticleObject->Get_ListParticleSampleSize());

	m_pParticleObject->Add_ParticleSample(tParticleSample);



	m_cListParticleSample.AddString(wszSampleName);

	TestSave();
	TestLoad();
}

//파티클 샘플을 삭제하는 함수
void CTool_Particle::OnBnClickedMwButtonDeleteSample()
{
	if (m_iSampleSelect < 0)
	{
		return;
	}

	m_pParticleObject->Delete_ParticleSample(m_iSampleSelect);
	TestSave();
	TestLoad();
	DataReLoad();
}

//대기중인 스포너 리스트에서 스포너를 삭제하는 함수
void CTool_Particle::OnBnClickedMwDeleteSpawner()
{
	if (m_iSpawnerSelect < 0)
	{
		return;
	}

	m_pParticleObject->Delete_WaitSpawner(m_iSpawnerSelect);
	TestSave();
	TestLoad();
	DataReLoad();
}

//대기중인 스포너 리스트에서 스포너를 추가하는 함수
void CTool_Particle::OnBnClickedMwAddSpawner()
{
	T_PARTICLE_SPAWNER tSpawner;

	if (!m_bPasteSpawner)
	{
		ZeroMemory(&tSpawner, sizeof(T_PARTICLE_SPAWNER));
		tSpawner.fLifeTime = 1.f;
		tSpawner.fSpawnInterval = 0.5f;
	}
	else
	{
		tSpawner = m_tCopyParticleSpawner;
	}

	WCHAR wszSampleName[32];
	swprintf_s(wszSampleName, L"Wait Spawner %d", m_pParticleObject->Get_ListWaitSpawnerSize());

	m_pParticleObject->Add_WaitSpawner(tSpawner);
	m_cListWaitSpawner.AddString(wszSampleName);

	TestSave();
	TestLoad();
}

//텍스쳐 리스트에서 텍스쳐를 삭제하는 함수
void CTool_Particle::OnBnClickedMwDeleteTexture()
{
	if (m_iTextureSelect < 0)
	{
		return;
	}

	m_pParticleObject->Delete_Texture(m_iTextureSelect);
	TestSave();
	TestLoad();
	DataReLoad();
}

//텍스쳐 리스트에서 텍스쳐를 추가하는 함수
void CTool_Particle::OnBnClickedMwButtonAddTexture()
{
	if (m_wstrTexName.size() <= 0)
		return;
	wstring wstrFullPath = m_wstrTexPath + m_wstrTexName;
	m_pParticleObject->Add_Texture(wstrFullPath);
	m_cListTexture.AddString(m_wstrTexName.c_str());
	
	TestSave();
	TestLoad();
}

//파티클 오브젝트에 기록된 모든 정보 (파티클 샘플, 대기중인 스포너, 텍스쳐)를 저장하는 함수
void CTool_Particle::OnBnClickedMwButtonSaveParticle()
{
	CFileDialog Dlg(FALSE, L"particle", L"*.particle", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Particle\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}
		DWORD dwByte;
		m_pParticleObject->Save(hFile, dwByte);
		CloseHandle(hFile);

		CPrefabManager::GetInstance()->LoadFile();
	}
}

//파티클 오브젝트의 모든 정보를 불러오는 함수
void CTool_Particle::OnBnClickedMwLoadParticle()
{
	CFileDialog Dlg(TRUE, L"particle", L"*.particle", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Particle\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}
		DWORD dwByte;

		m_pParticleObject->Load(hFile, dwByte);
	
		DataReLoad();

		CloseHandle(hFile);

		
	}
	TestSave();
	TestLoad();
}

//Combo box에 선택지 추가
BOOL CTool_Particle::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_cComboSampleType.AddString(L"Absolute");
	m_cComboSampleType.AddString(L"Local_Rotation");
	m_cComboSampleType.AddString(L"Local_NoneRotation");
	m_cComboSampleType.AddString(L"Center");
	
	m_cComboSpawnerType.AddString(L"Absolute");
	m_cComboSpawnerType.AddString(L"Local");
	//m_cComboSpawnerType.AddString(L"Center");
	return TRUE;
}

//파티클 오브젝트의 정보를 파티클툴에 다시 불러오는 함수
void CTool_Particle::DataReLoad()
{
	UpdateData(true);
	if (!m_pParticleObject)
	{
		return;
	}

	m_cListParticleSample.ResetContent();
	m_cListWaitSpawner.ResetContent();
	m_cListTexture.ResetContent();

	WCHAR wszName[32];

	_int iSize = m_pParticleObject->Get_ListParticleSampleSize();
	for (_int i = 0; i < iSize; ++i)
	{
		swprintf_s(wszName, L"Particle Sample %d", i);
		m_cListParticleSample.AddString(wszName);
	}

	WaitSpanwerReLoad();

	iSize = m_pParticleObject->Get_vecTextureSize();

	for (_int i = 0; i < iSize; ++i)
	{
		wstring wstrFullPath = m_pParticleObject->Get_TexturePath(i).c_str();
		_int iCheck = (_int)wstrFullPath.rfind(L"/");
		wstring wstrFileName = wstrFullPath.substr(iCheck+1);
		m_cListTexture.AddString(wstrFileName.c_str());
	}

	m_strParticleObjectLifeTime = CUtility::RoundWString(to_wstring(m_pParticleObject->Get_LifeTime()), 2).c_str();
	m_strParticleObjectScale = CUtility::RoundWString(to_wstring(m_pParticleObject->Get_Scale().x), 2).c_str();

	m_strParticleObjectRotX = CUtility::RoundWString(to_wstring(m_pParticleObject->Get_Angle().x), 2).c_str();
	m_strParticleObjectRotY = CUtility::RoundWString(to_wstring(m_pParticleObject->Get_Angle().y), 2).c_str();
	m_strParticleObjectRotZ = CUtility::RoundWString(to_wstring(m_pParticleObject->Get_Angle().z), 2).c_str();
	UpdateData(false);
	
}

//대기중인 스포너 정보를 다시 로드하는 함수
void CTool_Particle::WaitSpanwerReLoad()
{

	WCHAR wszName[32];

	_int iSize = m_pParticleObject->Get_ListWaitSpawnerSize();
	for (_int i = 0; i < iSize; ++i)
	{
		swprintf_s(wszName, L"Wait Spawner %d", i);
		m_cListWaitSpawner.AddString(wszName);
	}
}

//현재 작업중인 파티클 오브젝트를 관찰할 수 있도록 그려주는 함수
void CTool_Particle::ViewRender(_float fDeltaTime)
{

	Management GetDevice()->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f),
		1.f,
		0);
	Management GetDevice()->BeginScene();


	if (m_pViewParticle)
	{
		m_pViewParticle->Update_Object(fDeltaTime);
		m_pViewParticle->Render_Object();
		DebugRender();
		if (m_pViewParticle->Get_Dead())
		{
			TestLoad();
		}
		
	}

	Management GetDevice()->EndScene();
	Management GetDevice()->Present(NULL, NULL, m_cPicture.m_hWnd, NULL);
	
}

//현재 작업사항을 저장하는 함수
void CTool_Particle::TestSave()
{
	wstring wstrFullPath = L"..\\..\\Reference\\Data\\Particle\\TestParticle.particle";
	HANDLE hFile = CreateFile(wstrFullPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}
	DWORD dwByte;
	m_pParticleObject->Save(hFile, dwByte);
	CloseHandle(hFile);

	
}

//현재 작업사항을 관찰할 수 있는 오브젝트에 불러오는 함수
void CTool_Particle::TestLoad()
{
	wstring wstrFullPath = L"..\\..\\Reference\\Data\\Particle\\TestParticle.particle";
	HANDLE hFile = CreateFile(wstrFullPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}
	DWORD dwByte;
	if (m_pViewParticle)
	{
		m_pViewParticle->Set_Dead(TRUE);
		Safe_Release(m_pViewParticle);
	}
	m_pViewParticle = CParticleObject::Create(Management GetDevice(),E_OBJTAG::OBJ_Particle);
	m_pViewParticle->Load(hFile, dwByte);
	Management Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pViewParticle);
	m_pViewParticle->AddRef();
	CloseHandle(hFile);
}

//현재 활성화 중인 스포너의 위치를 렌더링 해주는 함수
void CTool_Particle::DebugRender()
{
	if (!m_pViewParticle)
	{
		return;
	}

	_matrix		matWorld, matView, matProj;
	_int iSize = m_pViewParticle->Get_ListActiveSpawnerSize();

	

	for (_int i = 0; i < iSize; ++i)
	{
		T_PARTICLE_SPAWNER* pSpawner = m_pViewParticle->Get_ActiveSpawnerPtr(i);
		if (!pSpawner)
		{
			return;
		}
		_matrix matScale;
		_matrix matTrans;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, pSpawner->vPos.x, pSpawner->vPos.y, pSpawner->vPos.z);
		matWorld = matScale*matTrans*static_cast<CTransform*>(m_pViewParticle->Get_Component(ComTag_Transform))->Get_WorldMatrix();
		
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

		Management GetDevice()->GetTransform(D3DTS_VIEW, &matView);
		Management GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);

		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(0);


		m_pDebugMesh->DrawSubset(0);


		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();
	}
}



void CTool_Particle::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}


void CTool_Particle::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	
	
}

//준비작업
int CTool_Particle::OnCreate(LPCREATESTRUCT lpCreateStruct)
{


	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_pParticleObject = CParticleObject::Create(Management GetDevice(), E_OBJTAG::OBJ_Particle);
	TestSave();

	D3DXCreateSphere(Management GetDevice(), 0.3f, 20, 4, &m_pDebugMesh, nullptr);

	m_pShaderCom = static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base"));
	

	return 0;
}

//메모리 해제
void CTool_Particle::OnDestroy()
{
	CDialogEx::OnDestroy();
	Safe_Release(m_pParticleObject);
	Safe_Release(m_pViewParticle);
	Safe_Release(m_pDebugMesh);
	Safe_Release(m_pShaderCom);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//파티클 샘플 리스트에서 셀을 선택했을때 해당 파티클 샘플 정보가 출력되도록 하는 함수
void CTool_Particle::OnLbnSelchangeMwSampleList()
{
	UpdateData(TRUE);
	

	m_iSampleSelect = m_cListParticleSample.GetCurSel();
	T_PARTICLE* pParticle=nullptr;
	if (0 <= m_iSampleSelect)
	{
		pParticle = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (!pParticle)
	{
		return;
	}

	m_strSampleAddMinPosX.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMinAddPos.x), 2).c_str());
	m_strSampleAddMinPosY.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMinAddPos.y), 2).c_str());
	m_strSampleAddMinPosZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMinAddPos.z), 2).c_str());
					
	m_strSampleAddMaxPosX.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMaxAddPos.x), 2).c_str());
	m_strSampleAddMaxPosY.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMaxAddPos.y), 2).c_str());
	m_strSampleAddMaxPosZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMaxAddPos.z), 2).c_str());

	m_strSampleAddMinRotX.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMinAddRot.x), 2).c_str());
	m_strSampleAddMinRotY.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMinAddRot.y), 2).c_str());
	m_strSampleAddMinRotZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMinAddRot.z), 2).c_str());

	m_strSampleAddMaxRotX.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMaxAddRot.x), 2).c_str());
	m_strSampleAddMaxRotY.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMaxAddRot.y), 2).c_str());
	m_strSampleAddMaxRotZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vRandomMaxAddRot.z), 2).c_str());


	m_strSampleScaleX.SetString(CUtility::RoundWString(to_wstring(pParticle->vScale.x), 2).c_str());
	m_strSampleScaleY.SetString(CUtility::RoundWString(to_wstring(pParticle->vScale.y), 2).c_str());
	m_strSampleScaleZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vScale.z), 2).c_str());

	m_strSampleMaxScaleX.SetString(CUtility::RoundWString(to_wstring(pParticle->vScaleMax.x), 2).c_str());
	m_strSampleMaxScaleY.SetString(CUtility::RoundWString(to_wstring(pParticle->vScaleMax.y), 2).c_str());
	m_strSampleMaxScaleZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vScaleMax.z), 2).c_str());

	m_strSampleAddMinScaleX.SetString(CUtility::RoundWString(to_wstring(pParticle->vEndScale.x), 2).c_str());
	m_strSampleAddMinScaleY.SetString(CUtility::RoundWString(to_wstring(pParticle->vEndScale.y), 2).c_str());
	m_strSampleAddMinScaleZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vEndScale.z), 2).c_str());

	D3DXCOLOR vColor = D3DXCOLOR(pParticle->vStartColor.x, pParticle->vStartColor.y, pParticle->vStartColor.z, 1.f);
	m_cColorStart.SetColor(D3DCOLORTOCOLORREF(vColor));
	m_strSampleColorA.SetString(CUtility::RoundWString(to_wstring(pParticle->vStartColor.w), 2).c_str());
	
	vColor = D3DXCOLOR(pParticle->vStartColorMax.x, pParticle->vStartColorMax.y, pParticle->vStartColorMax.z, 1.f);
	m_cColorStartMax.SetColor(D3DCOLORTOCOLORREF(vColor));
	m_strSampleMaxAlpha.SetString(CUtility::RoundWString(to_wstring(pParticle->vStartColorMax.w), 2).c_str());

	vColor = D3DXCOLOR(pParticle->vEndColor.x, pParticle->vEndColor.y, pParticle->vEndColor.z, 1.f);
	m_cColorEnd.SetColor(D3DCOLORTOCOLORREF(vColor));
	m_strSampleAddMinColorA.SetString(CUtility::RoundWString(to_wstring(pParticle->vEndColor.w), 2).c_str());
	
	m_strSampleLifeTime.SetString(CUtility::RoundWString(to_wstring(pParticle->fLifeTime), 2).c_str());
	m_strSampleLifeTimeMax.SetString(CUtility::RoundWString(to_wstring(pParticle->fMaxLifeTime), 2).c_str());
	m_strSampleTexIndex.SetString(to_wstring(pParticle->iTextureIndex).c_str());

	m_strSampleGravity.SetString(CUtility::RoundWString(to_wstring(pParticle->fGravity), 2).c_str());
	m_strSampleMaxGravity.SetString(CUtility::RoundWString(to_wstring(pParticle->fMaxGravity), 2).c_str());
	m_cComboSampleType.SetCurSel(_int(pParticle->eCoordSystem));

	m_bSampleBillBoard = pParticle->bBillBoard;
	m_bIndependent = pParticle->bIndependent;
	m_bBlackBackGroundImage = pParticle->bBlackBackGroundImage;
	m_bUseCross = pParticle->bUseCross;
	m_bBillBoardLookDir = pParticle->bBillBoardLookDir;

	m_strBillBoardZRot.SetString(CUtility::RoundWString(to_wstring(pParticle->fBillboardZRot), 2).c_str());

	m_strSampleOffsetRotX.SetString(CUtility::RoundWString(to_wstring(pParticle->vOffSetRot.x), 2).c_str());
	m_strSampleOffsetRotY.SetString(CUtility::RoundWString(to_wstring(pParticle->vOffSetRot.y), 2).c_str());
	m_strSampleOffsetRotZ.SetString(CUtility::RoundWString(to_wstring(pParticle->vOffSetRot.z), 2).c_str());

	m_iRadioAlphaOption = pParticle->iAlphaOption;
	m_bSampleBlur = pParticle->bBlur;
	m_bSampleZEnable = pParticle->bZEnable;


	m_strSpriteX = to_wstring(pParticle->iSpriteX).c_str();
	m_strSpriteY = to_wstring(pParticle->iSpriteY).c_str();
	m_strCycleNum.SetString(CUtility::RoundWString(to_wstring(pParticle->fCycle), 2).c_str());

	UpdateData(FALSE);

	TestSave();
	TestLoad();
}

//대기중인 스포너 리스트에서 셀을 선택했을때 해당 스포너 정보가 출력되도록 하는 함수
void CTool_Particle::OnLbnSelchangeMwWaitSpawnerList()
{
	UpdateData(true);


	m_iSpawnerSelect = m_cListWaitSpawner.GetCurSel();

	if (m_iSpawnerSelect < 0)
	{
		return;
	}
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);

	if (!pSpawner)
	{
		_int iA = 123;
		return;
	}

	m_strSpawnerPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vPos.x), 2).c_str());
	m_strSpawnerPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vPos.y), 2).c_str());
	m_strSpawnerPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vPos.z), 2).c_str());

	m_strSpawnerAddMinPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinAddPos.x), 2).c_str());
	m_strSpawnerAddMinPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinAddPos.y), 2).c_str());
	m_strSpawnerAddMinPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinAddPos.z), 2).c_str());

	m_strSpawnerAddMaxPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxAddPos.x), 2).c_str());
	m_strSpawnerAddMaxPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxAddPos.y), 2).c_str());
	m_strSpawnerAddMaxPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxAddPos.z), 2).c_str());


	m_strSpawnerRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRot.x), 2).c_str());
	m_strSpawnerRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRot.y), 2).c_str());
	m_strSpawnerRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRot.z), 2).c_str());

	m_strSpawnerAddMinRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinAddRot.x), 2).c_str());
	m_strSpawnerAddMinRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinAddRot.y), 2).c_str());
	m_strSpawnerAddMinRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinAddRot.z), 2).c_str());

	m_strSpawnerAddMaxRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxAddRot.x), 2).c_str());
	m_strSpawnerAddMaxRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxAddRot.y), 2).c_str());
	m_strSpawnerAddMaxRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxAddRot.z), 2).c_str());


	m_strSpawnerSpawnMinPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinSpawnPos.x), 2).c_str());
	m_strSpawnerSpawnMinPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinSpawnPos.y), 2).c_str());
	m_strSpawnerSpawnMinPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinSpawnPos.z), 2).c_str());

	m_strSpawnerSpawnMaxPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxSpawnPos.x), 2).c_str());
	m_strSpawnerSpawnMaxPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxSpawnPos.y), 2).c_str());
	m_strSpawnerSpawnMaxPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxSpawnPos.z), 2).c_str());


	m_strSpawnerLifeTime.SetString(CUtility::RoundWString(to_wstring(pSpawner->fLifeTime), 2).c_str());
	m_strSpawnerEntranceTime.SetString(CUtility::RoundWString(to_wstring(pSpawner->fEntranceTime), 2).c_str());
	m_strSpawnerSpawnInterval.SetString(CUtility::RoundWString(to_wstring(pSpawner->fSpawnInterval), 2).c_str());
	m_strSpawnerSampleIndex.SetString(to_wstring(pSpawner->iSpawnSampleIndex).c_str());

	m_strSpawnerMultiCount.SetString(to_wstring(pSpawner->iMultiSpawnCount).c_str());
	m_bSpawnerMultiSpawn = pSpawner->bMultiSpawn;

	m_strSpawnerMultiStartPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vStartPos.x), 2).c_str());
	m_strSpawnerMultiStartPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vStartPos.y), 2).c_str());
	m_strSpawnerMultiStartPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vStartPos.z), 2).c_str());

	m_strSpawnerMultiStartRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vStartRot.x), 2).c_str());
	m_strSpawnerMultiStartRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vStartRot.y), 2).c_str());
	m_strSpawnerMultiStartRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vStartRot.z), 2).c_str());

	m_strSpawnerMultiAddPosX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vAddPos.x), 2).c_str());
	m_strSpawnerMultiAddPosY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vAddPos.y), 2).c_str());
	m_strSpawnerMultiAddPosZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vAddPos.z), 2).c_str());

	m_strSpawnerMultiAddRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vAddRot.x), 2).c_str());
	m_strSpawnerMultiAddRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vAddRot.y), 2).c_str());
	m_strSpawnerMultiAddRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vAddRot.z), 2).c_str());

	m_strSpawnMinRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinSpawnRot.x), 2).c_str());
	m_strSpawnMinRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinSpawnRot.y), 2).c_str());
	m_strSpawnMinRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMinSpawnRot.z), 2).c_str());

	m_strSpawnMaxRotX.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxSpawnRot.x), 2).c_str());
	m_strSpawnMaxRotY.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxSpawnRot.y), 2).c_str());
	m_strSpawnMaxRotZ.SetString(CUtility::RoundWString(to_wstring(pSpawner->vRandomMaxSpawnRot.z), 2).c_str());

	m_cComboSpawnerType.SetCurSel(_int(pSpawner->eCoordSystem));
	UpdateData(false);
	TestSave();
	TestLoad();

}
//========================================= 이 밑으로는 툴에서 입력한 값을 실제 파티클 오브젝트의 데이터에 입력하는 함수  ==================================================
void CTool_Particle::OnEnChangeMwAddMinPosx()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMinAddPos.x = (_float)_wtof(m_strSampleAddMinPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinPosy()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMinAddPos.y = (_float)_wtof(m_strSampleAddMinPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinPosz()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMinAddPos.z = (_float)_wtof(m_strSampleAddMinPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMaxPosx()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMaxAddPos.x = (_float)_wtof(m_strSampleAddMaxPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMaxPosy()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMaxAddPos.y = (_float)_wtof(m_strSampleAddMaxPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMaxPosz()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMaxAddPos.z = (_float)_wtof(m_strSampleAddMaxPosZ.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwAddMinRotx()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMinAddRot.x = (_float)_wtof(m_strSampleAddMinRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinRoty()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMinAddRot.y = (_float)_wtof(m_strSampleAddMinRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinRotz()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMinAddRot.z = (_float)_wtof(m_strSampleAddMinRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMaxRotx()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMaxAddRot.x = (_float)_wtof(m_strSampleAddMaxRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMaxRoty()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMaxAddRot.y = (_float)_wtof(m_strSampleAddMaxRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMaxRotz()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vRandomMaxAddRot.z = (_float)_wtof(m_strSampleAddMaxRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwScalex()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vScale.x = (_float)_wtof(m_strSampleScaleX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwScaley()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vScale.y = (_float)_wtof(m_strSampleScaleY.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwScalez()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vScale.z = (_float)_wtof(m_strSampleScaleZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinScalex()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vEndScale.x = (_float)_wtof(m_strSampleAddMinScaleX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinScaley()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vEndScale.y = (_float)_wtof(m_strSampleAddMinScaleY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwAddMinScalez()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vEndScale.z = (_float)_wtof(m_strSampleAddMinScaleZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSampleA()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vStartColor.w = (_float)_wtof(m_strSampleColorA.GetString());
	}
	TestSave();
	TestLoad();
}




void CTool_Particle::OnEnChangeMwEditSampleAddMinColorA()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >=0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->vEndColor.w = (_float)_wtof(m_strSampleAddMinColorA.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwEditSampleLifeTime()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->fLifeTime = (_float)_wtof(m_strSampleLifeTime.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMw1217EditSampleIndex()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->iTextureIndex = _wtoi(m_strSampleTexIndex.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1219EditGravity()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->fGravity = (_float)_wtof(m_strSampleGravity.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1219EditMaxGravity()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		pParticleSample->fMaxGravity= (_float)_wtof(m_strSampleMaxGravity.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnBnClickedMw1219CheckSampleBillboard()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pParticleSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pParticleSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pParticleSample)
	{
		if (m_bSampleBillBoard)
		{
			pParticleSample->bBillBoard = TRUE;
		}
		else
		{
			pParticleSample->bBillBoard = FALSE;
		}
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwEditSpawnerPosX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vPos.x = (_float)_wtof(m_strSpawnerPosX.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwEditSpawnerPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vPos.y = (_float)_wtof(m_strSpawnerPosY.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwEditSpawnerPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vPos.z = (_float)_wtof(m_strSpawnerPosZ.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMwEditSpawnerRotX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRot.x = (_float)_wtof(m_strSpawnerRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRot.y = (_float)_wtof(m_strSpawnerRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRot.z = (_float)_wtof(m_strSpawnerRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerSpawnMinPosX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinSpawnPos.x = (_float)_wtof(m_strSpawnerSpawnMinPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerSpawnMinPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinSpawnPos.y = (_float)_wtof(m_strSpawnerSpawnMinPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerSpawnMinPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinSpawnPos.z = (_float)_wtof(m_strSpawnerSpawnMinPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerSpawnMaxPosX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxSpawnPos.x = (_float)_wtof(m_strSpawnerSpawnMaxPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerSpawnMaxPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxSpawnPos.y = (_float)_wtof(m_strSpawnerSpawnMaxPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerSpawnMaxPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxSpawnPos.z = (_float)_wtof(m_strSpawnerSpawnMaxPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerLifeTime()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->fLifeTime = (_float)_wtof(m_strSpawnerLifeTime.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditSpawnerEntranceTime()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->fEntranceTime = (_float)_wtof(m_strSpawnerEntranceTime.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMwEditParticleObjectLifeTime()
{

	UpdateData(true);
	UpdateData(false);
	
	m_pParticleObject->Set_LifeTime((_float)_wtof(m_strParticleObjectLifeTime.GetString()));
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMinPosx()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinAddPos.x = (_float)_wtof(m_strSpawnerAddMinPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMinPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinAddPos.y = (_float)_wtof(m_strSpawnerAddMinPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMinPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinAddPos.z = (_float)_wtof(m_strSpawnerAddMinPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMaxPosX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxAddPos.x = (_float)_wtof(m_strSpawnerAddMaxPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMaxPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxAddPos.y = (_float)_wtof(m_strSpawnerAddMaxPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMaxPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxAddPos.z = (_float)_wtof(m_strSpawnerAddMaxPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMinRotX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinAddRot.x = (_float)_wtof(m_strSpawnerAddMinRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMinRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinAddRot.y= (_float)_wtof(m_strSpawnerAddMinRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMinRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinAddRot.z = (_float)_wtof(m_strSpawnerAddMinRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMaxRotX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxAddRot.x = (_float)_wtof(m_strSpawnerAddMaxRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMaxRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxAddRot.y = (_float)_wtof(m_strSpawnerAddMaxRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1216SpawnerAddMaxRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxAddRot.z = (_float)_wtof(m_strSpawnerAddMaxRotZ.GetString());
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMw1217EditSpawnerSpawnInterval()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->fSpawnInterval = (_float)_wtof(m_strSpawnerSpawnInterval.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1217EditSpawnerSampleIndex()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->iSpawnSampleIndex = _wtoi(m_strSpawnerSampleIndex.GetString());
	}
	TestSave();
	TestLoad();
}




void CTool_Particle::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	DataReLoad();
	TestSave();
	TestLoad();
}





void CTool_Particle::OnLbnSelchangeMwTextureList()
{
	UpdateData(true);
	UpdateData(false);

	m_iTextureSelect = m_cListTexture.GetCurSel();


}




void CTool_Particle::OnCbnSelchangeMwComboSpawnerSystem()
{
	UpdateData(true);
	UpdateData(false);

	_int iSelect = m_cComboSpawnerType.GetCurSel();

	//m_pParticleObject->Get_WaitSpawnerPtr[]
	if (m_iSpawnerSelect < 0)
	{
		return;
	}
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	if (pSpawner)
	{
		pSpawner->eCoordSystem = E_COORD_SYSTEM(iSelect);
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnCbnSelchangeMwSampleSystemCombo()
{
	UpdateData(true);
	UpdateData(false);

	_int iSelect = m_cComboSampleType.GetCurSel();

	if (m_iSampleSelect < 0)
	{
		return;
	}
	T_PARTICLE* pSample = nullptr;
	pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	if (pSample)
	{
		pSample->eCoordSystem = E_COORD_SYSTEM(iSelect);
	}
	TestSave();
	TestLoad();
}

void CTool_Particle::OnBnClickedMw1220CehckMultiple()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		if (m_bSpawnerMultiSpawn)
		{
			pSpawner->bMultiSpawn = TRUE;
		}
		else
		{
			pSpawner->bMultiSpawn = FALSE;
		}
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditMultiCount()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->iMultiSpawnCount = _wtoi(m_strSpawnerMultiCount.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartPosX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vStartPos.x = (_float)_wtoi(m_strSpawnerMultiStartPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vStartPos.y = (_float)(_float)_wtof(m_strSpawnerMultiStartPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vStartPos.z = (_float)_wtof(m_strSpawnerMultiStartPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartRotX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vStartRot.x = (_float)_wtof(m_strSpawnerMultiStartRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vStartRot.y = (_float)_wtof(m_strSpawnerMultiStartRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiStartRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vStartRot.z = (_float)_wtof(m_strSpawnerMultiStartRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddPosX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vAddPos.x = (_float)_wtof(m_strSpawnerMultiAddPosX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddPosY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vAddPos.y = (_float)_wtof(m_strSpawnerMultiAddPosY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddPosZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vAddPos.z = (_float)_wtof(m_strSpawnerMultiAddPosZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddRotX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vAddRot.x = (_float)_wtof(m_strSpawnerMultiAddRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vAddRot.y = (_float)_wtof(m_strSpawnerMultiAddRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw1220EditSpawnerMultiAddRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vAddRot.z = (_float)_wtof(m_strSpawnerMultiAddRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0107EditParticleObjectScale()
{
	UpdateData(true);
	UpdateData(false);
	
	_float fScale = (_float)_wtof(m_strParticleObjectScale.GetString());
	m_pParticleObject->Set_Scale(_vec3(fScale, fScale, fScale));

	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0107ButtonSpawnerCopy()
{
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		m_tCopyParticleSpawner = *pSpawner;
	}
	
}


void CTool_Particle::OnBnClickedMw0107PasteSpawner()
{
	m_bPasteSpawner = TRUE;
	OnBnClickedMwAddSpawner();
	m_bPasteSpawner = FALSE;
}


void CTool_Particle::OnBnClickedMw0107ButtonCopySample()
{
	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		m_tCopyParticleSample = *pSample;
	}
}


void CTool_Particle::OnBnClickedMw0107ButtonPasteSample()
{
	m_bPasteSample = TRUE;
	OnBnClickedMwButtonAddSample();
	m_bPasteSample = FALSE;
}


void CTool_Particle::OnBnClickedMw0107ColorStart()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}

	COLORREF colorStart = m_cColorStart.GetColor();
	if (pSample)
	{
		_float fR, fG, fB;
		fR = GetRGB255_TO_RGB1f(GetRValue(colorStart));
		fR = SetFloatColor(fR);
		fG = GetRGB255_TO_RGB1f(GetGValue(colorStart));
		fG = SetFloatColor(fG);
		fB = GetRGB255_TO_RGB1f(GetBValue(colorStart));
		fB = SetFloatColor(fB);

		pSample->vStartColor.x = fR;
		pSample->vStartColor.y = fG;
		pSample->vStartColor.z = fB;
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0107ColorEnd()
{
	UpdateData(true);
	UpdateData(false);


	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}

	COLORREF colorEnd = m_cColorEnd.GetColor();

	if (pSample)
	{
		_float fR, fG, fB;
		fR = GetRGB255_TO_RGB1f(GetRValue(colorEnd));
		fR = SetFloatColor(fR);
		fG = GetRGB255_TO_RGB1f(GetGValue(colorEnd));
		fG = SetFloatColor(fG);
		fB = GetRGB255_TO_RGB1f(GetBValue(colorEnd));
		fB = SetFloatColor(fB);

		pSample->vEndColor.x = fR;
		pSample->vEndColor.y = fG;
		pSample->vEndColor.z = fB;

	}

	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0107EditParticleObjectRotX()
{
	UpdateData(true);
	UpdateData(false);
	m_pParticleObject->Set_Rotation(E_ROTATION::ROT_X, (_float)_wtof(m_strParticleObjectRotX.GetString()));
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0107EditParticleObjectRotY()
{
	UpdateData(true);
	UpdateData(false);
	m_pParticleObject->Set_Rotation(E_ROTATION::ROT_Y, (_float)_wtof(m_strParticleObjectRotY.GetString()));
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0107EditParticleObjectRotZ()
{
	UpdateData(true);
	UpdateData(false);
	m_pParticleObject->Set_Rotation(E_ROTATION::ROT_Z, (_float)_wtof(m_strParticleObjectRotZ.GetString()));
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0107CheckIndependent()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		if (m_bIndependent)
		{
			pSample->bIndependent = TRUE;
		}
		else
		{
			pSample->bIndependent = FALSE;
		}
		
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0114CheckBlackImage()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		if (m_bBlackBackGroundImage)
		{
			pSample->bBlackBackGroundImage = TRUE;
		}
		else
		{
			pSample->bBlackBackGroundImage = FALSE;
		}

	}

	TestSave();
	TestLoad();
}



void CTool_Particle::OnBnClickedMw0119CheckUseCross()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		if (m_bUseCross)
		{
			pSample->bUseCross = TRUE;
		}
		else
		{
			pSample->bUseCross = FALSE;
		}

	}

	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0119CheckBillboardLookDir()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		if (m_bBillBoardLookDir)
		{
			pSample->bBillBoardLookDir = TRUE;
		}
		else
		{
			pSample->bBillBoardLookDir = FALSE;
		}

	}

	TestSave();
	TestLoad();
}

void CTool_Particle::OnEnChangeMw0119MaxScalex()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vScaleMax.x = (_float)_wtof(m_strSampleMaxScaleX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119MaxScaley()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vScaleMax.y = (_float)_wtof(m_strSampleMaxScaleY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119MaxScalez()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vScaleMax.z = (_float)_wtof(m_strSampleMaxScaleZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditOffsetRotX()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vOffSetRot.x = (_float)_wtof(m_strSampleOffsetRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditOffsetRotY()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vOffSetRot.y = (_float)_wtof(m_strSampleOffsetRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditOffsetRotZ()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vOffSetRot.z = (_float)_wtof(m_strSampleOffsetRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditBillboardZRot()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->fBillboardZRot = (_float)_wtof(m_strBillBoardZRot.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditMaxAlpha()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->vStartColorMax.w = (_float)_wtof(m_strSampleMaxAlpha.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0119ColorStartMax()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}

	COLORREF colorStart = m_cColorStartMax.GetColor();
	if (pSample)
	{
		_float fR, fG, fB;
		fR = GetRGB255_TO_RGB1f(GetRValue(colorStart));
		fR = SetFloatColor(fR);
		fG = GetRGB255_TO_RGB1f(GetGValue(colorStart));
		fG = SetFloatColor(fG);
		fB = GetRGB255_TO_RGB1f(GetBValue(colorStart));
		fB = SetFloatColor(fB);

		pSample->vStartColorMax.x = fR;
		pSample->vStartColorMax.y = fG;
		pSample->vStartColorMax.z = fB;
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMinRotX()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinSpawnRot.x = (_float)_wtof(m_strSpawnMinRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMinRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinSpawnRot.y = (_float)_wtof(m_strSpawnMinRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMinRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMinSpawnRot.z = (_float)_wtof(m_strSpawnMinRotZ.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMaxRot()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxSpawnRot.x = (_float)_wtof(m_strSpawnMaxRotX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMaxRotY()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxSpawnRot.y = (_float)_wtof(m_strSpawnMaxRotY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0119EditSpawnerSpawnMaxRotZ()
{
	UpdateData(true);
	UpdateData(false);
	T_PARTICLE_SPAWNER* pSpawner = nullptr;
	if (m_iSpawnerSelect >= 0)
	{
		pSpawner = m_pParticleObject->Get_WaitSpawnerPtr(m_iSpawnerSelect);
	}
	if (pSpawner)
	{
		pSpawner->vRandomMaxSpawnRot.z = (_float)_wtof(m_strSpawnMaxRotZ.GetString());
	}
	TestSave();
	TestLoad();
}





void CTool_Particle::OnBnClickedMw0120RadioAlphaTrans()
{
	UpdateData(true);
	m_iRadioAlphaOption = 0;
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->iAlphaOption = m_iRadioAlphaOption;
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0120RadioAlphaBlend()
{
	UpdateData(true);
	m_iRadioAlphaOption = 1;
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->iAlphaOption = m_iRadioAlphaOption;
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0120RadioNonAlpha()
{
	UpdateData(true);
	m_iRadioAlphaOption = 2;
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->iAlphaOption = m_iRadioAlphaOption;
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0120CheckBlur()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		if (m_bSampleBlur)
		{
			pSample->bBlur = TRUE;
		}
		else
		{
			pSample->bBlur = FALSE;
		}

	}

	TestSave();
	TestLoad();
}


void CTool_Particle::OnBnClickedMw0120CheckZenable()
{
	UpdateData(true);
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		if (m_bSampleZEnable)
		{
			pSample->bZEnable = TRUE;
		}
		else
		{
			pSample->bZEnable = FALSE;
		}

	}

	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0120EditSpriteX()
{
	UpdateData(true);

	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		
		pSample->iSpriteX = _wtoi(m_strSpriteX.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0120EditSpriteY()
{
	UpdateData(true);

	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{

		pSample->iSpriteY = _wtoi(m_strSpriteY.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0120EditSpriteCycle()
{
	UpdateData(true);
	
	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->fCycle = (_float)_wtof(m_strCycleNum.GetString());
	}
	TestSave();
	TestLoad();
}


void CTool_Particle::OnEnChangeMw0121EditSampleLifeTimeMax()
{
	UpdateData(true);

	UpdateData(false);

	T_PARTICLE* pSample = nullptr;
	if (m_iSampleSelect >= 0)
	{
		pSample = m_pParticleObject->Get_ParticleSamplePtr(m_iSampleSelect);
	}
	if (pSample)
	{
		pSample->fMaxLifeTime = (_float)_wtof(m_strSampleLifeTimeMax.GetString());
	}
	TestSave();
	TestLoad();
}

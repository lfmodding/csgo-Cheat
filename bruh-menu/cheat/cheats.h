#pragma once
#include <Windows.h>

#include "memoryManager.h"



class Cheats
{
private:
	
	//actual cheating functions
	void simulateKeyPress(int VK_KEY);
	bool getPlayerShot(HANDLE hProcess, uintptr_t moduleBase);
	int getPlayerTeam(uintptr_t player, HANDLE hProcess);
	uintptr_t getLocalPlayer(HANDLE hProcess, uintptr_t moduleBase);
	uintptr_t getPlayer(int index, HANDLE hProcess, uintptr_t moduleBase);
	void playerShoot(HANDLE hProcess, uintptr_t moduleBase, bool switchWeapon, int fireTime, int pauseTime);
	void playerForceJump(HANDLE hProcess, uintptr_t moduleBase);
	int getPlayerHealth(uintptr_t player, HANDLE hProcess);
	Vector3 getPlayerLocation(uintptr_t player, HANDLE hProcess);
	bool getPlayerDormant(uintptr_t player, HANDLE hProcess);
	bool getPlayerInCrosshair(uintptr_t player, HANDLE hProcess, uintptr_t moduleBase);
	Vector3 getPlayerHead(uintptr_t player, HANDLE hProcess);
	int getPlayerFlags(HANDLE hProcess, uintptr_t moduleBase);
	
	//math
	Vector3 CreateStartPoint(const Vector3 playerHeadPosition, int randomHeight);
	float pythag(int x1, int y1, int x2, int y2);
	int randomNumber(int maxNumber, int minNumber);
	
public:
	
	int FindClosestEnemy(HANDLE hProcess, uintptr_t moduleBase);
	void bHop(HANDLE hProcess, uintptr_t moduleBase);
	void radar(HANDLE hProcess, uintptr_t moduleBase);
	void triggerbot(HANDLE hProcess, uintptr_t moduleBase, int VK_BUTTON, int timeToShoot, bool switchWeapon, int shootTime, int pauseTime);
	void aimbot(HANDLE hProcess, int closest, uintptr_t moduleBase, int VK_KEY, int aimbotFOV, float aimbotSmoothing);
	void sniperAutoChangeWeapon(HANDLE hProcess, uintptr_t moduleBase);
	Vector3 ESP(HANDLE hProcess, uintptr_t moduleBase);
	
};





#define cs_gamerules_data 0x0
#define m_ArmorValue 0x117CC
#define m_Collision 0x320
#define m_CollisionGroup 0x474
#define m_Local 0x2FCC
#define m_MoveType 0x25C
#define m_OriginalOwnerXuidHigh 0x31D4
#define m_OriginalOwnerXuidLow 0x31D0
#define m_SurvivalGameRuleDecisionTypes 0x1328
#define m_SurvivalRules 0xD00
#define m_aimPunchAngle 0x303C
#define m_aimPunchAngleVel 0x3048
#define m_angEyeAnglesX 0x117D0
#define m_angEyeAnglesY 0x117D4
#define m_bBombDefused 0x29C0
#define m_bBombPlanted 0x9A5
#define m_bBombTicking 0x2990
#define m_bFreezePeriod 0x20
#define m_bGunGameImmunity 0x9990
#define m_bHasDefuser 0x117DC
#define m_bHasHelmet 0x117C0
#define m_bInReload 0x32B5
#define m_bIsDefusing 0x997C
#define m_bIsQueuedMatchmaking 0x74
#define m_bIsScoped 0x9974
#define m_bIsValveDS 0x7C
#define m_bSpotted 0x93D
#define m_bSpottedByMask 0x980
#define m_bStartedArming 0x3400
#define m_bUseCustomAutoExposureMax 0x9D9
#define m_bUseCustomAutoExposureMin 0x9D8
#define m_bUseCustomBloomScale 0x9DA
#define m_clrRender 0x70
#define m_dwBoneMatrix 0x26A8
#define m_fAccuracyPenalty 0x3340
#define m_fFlags 0x104
#define m_flC4Blow 0x29A0
#define m_flCustomAutoExposureMax 0x9E0
#define m_flCustomAutoExposureMin 0x9DC
#define m_flCustomBloomScale 0x9E4
#define m_flDefuseCountDown 0x29BC
#define m_flDefuseLength 0x29B8
#define m_flFallbackWear 0x31E0
#define m_flFlashDuration 0x10470
#define m_flFlashMaxAlpha 0x1046C
#define m_flLastBoneSetupTime 0x2928
#define m_flLowerBodyYawTarget 0x9ADC
#define m_flNextAttack 0x2D80
#define m_flNextPrimaryAttack 0x3248
#define m_flSimulationTime 0x268
#define m_flTimerLength 0x29A4
#define m_hActiveWeapon 0x2F08
#define m_hBombDefuser 0x29C4
#define m_hMyWeapons 0x2E08
#define m_hObserverTarget 0x339C
#define m_hOwner 0x29DC
#define m_hOwnerEntity 0x14C
#define m_hViewModel 0x3308
#define m_iAccountID 0x2FD8
#define m_iClip1 0x3274
#define m_iCompetitiveRanking 0x1A84
#define m_iCompetitiveWins 0x1B88
#define m_iCrosshairId 0x11838
#define m_iDefaultFOV 0x333C
#define m_iEntityQuality 0x2FBC
#define m_iFOV 0x31F4
#define m_iFOVStart 0x31F8
#define m_iGlowIndex 0x10488
#define m_iHealth 0x100
#define m_iItemDefinitionIndex 0x2FBA
#define m_iItemIDHigh 0x2FD0
#define m_iMostRecentModelBoneCounter 0x2690
#define m_iObserverMode 0x3388
#define m_iShotsFired 0x103E0
#define m_iState 0x3268
#define m_iTeamNum 0xF4
#define m_lifeState 0x25F
#define m_nBombSite 0x2994
#define m_nFallbackPaintKit 0x31D8
#define m_nFallbackSeed 0x31DC
#define m_nFallbackStatTrak 0x31E4
#define m_nForceBone 0x268C
#define m_nModelIndex 0x258
#define m_nTickBase 0x3440
#define m_nViewModelIndex 0x29D0
#define m_rgflCoordinateFrame 0x444
#define m_szCustomName 0x304C
#define m_szLastPlaceName 0x35C4
#define m_thirdPersonViewAngles 0x31E8
#define m_vecOrigin 0x138
#define m_vecVelocity 0x114
#define m_vecViewOffset 0x108
#define m_viewPunchAngle 0x3030
#define m_zoomLevel 0x33E0
#define anim_overlays 0x2990
#define clientstate_choked_commands 0x4D30
#define clientstate_delta_ticks 0x174
#define clientstate_last_outgoing_command 0x4D2C
#define clientstate_net_channel 0x9C
#define convar_name_hash_table 0x2F190
#define dwClientState 0x58CFDC
#define dwClientState_GetLocalPlayer 0x180
#define dwClientState_IsHLTV 0x4D48
#define dwClientState_Map 0x28C
#define dwClientState_MapDirectory 0x188
#define dwClientState_MaxPlayer 0x388
#define dwClientState_PlayerInfo 0x52C0
#define dwClientState_State 0x108
#define dwClientState_ViewAngles 0x4D90
#define dwEntityList 0x4DDB8FC
#define dwForceAttack 0x320BDC8
#define dwForceAttack2 0x320BDD4
#define dwForceBackward 0x320BE10
#define dwForceForward 0x320BE04
#define dwForceJump 0x52858A0
#define dwForceLeft 0x320BE1C
#define dwForceRight 0x320BE28
#define dwGameDir 0x62B900
#define dwGameRulesProxy 0x52F90BC
#define dwGetAllClasses 0xDE9C9C
#define dwGlobalVars 0x58CCE0
#define dwGlowObjectManager 0x5324590
#define dwInput 0x522CED0
#define dwInterfaceLinkList 0x96EF44
#define dwLocalPlayer 0xDBF4BC
#define dwMouseEnable 0xDC51C8
#define dwMouseEnablePtr 0xDC5198
#define dwPlayerResource 0x320A160
#define dwRadarBase 0x5210674
#define dwSensitivity 0xDC5064
#define dwSensitivityPtr 0xDC5038
#define dwSetClanTag 0x8A410
#define dwViewMatrix 0x4DCD214
#define dwWeaponTable 0x522D994
#define dwWeaponTableIndex 0x326C
#define dwYawPtr 0xDC4E28
#define dwZoomSensitivityRatioPtr 0xDCB610
#define dwbSendPackets 0xD8452
#define dwppDirect3DDevice9 0xA6050
#define find_hud_element 0x288959D0
#define force_update_spectator_glow 0x3BE2BA
#define interface_engine_cvar 0x3EA3C
#define is_c4_owner 0x3CB320
#define m_bDormant 0xED
#define m_bIsLocalPlayer 0x3628
#define m_flSpawnTime 0x103C0
#define m_pStudioHdr 0x2950
#define m_pitchClassPtr 0x5210910
#define m_yawClassPtr 0xDC4E28
#define model_ambient_min 0x590054
#define set_abs_angles 0x1E59F0
#define set_abs_origin 0x1E5830
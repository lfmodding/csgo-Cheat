#include "cheats.h"
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <TlHelp32.h>
#include <vector>


//screen
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN); const int xhairx = SCREEN_WIDTH / 2;
const int SCREEN_HEIGTH = GetSystemMetrics(SM_CYSCREEN); const int xhairy = SCREEN_HEIGTH / 2;

using namespace memoryManager;

struct view_matrix_t {
	float matrix[16];
} vm;

void Cheats::simulateKeyPress(int VK_KEY)
{
	INPUT input;
	WORD vkey = VK_KEY; // see link below
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(80);
}

bool Cheats::getPlayerShot(HANDLE hProcess, uintptr_t moduleBase)
{
	int playerShootEvent = RPM<int>(moduleBase + dwForceAttack, hProcess);
	if (playerShootEvent == 0x5)
		return true;
	else if (playerShootEvent == 0x4)
		return false;
}

int Cheats::getPlayerTeam(uintptr_t player, HANDLE hProcess)
{
	return RPM<int>(player + m_iTeamNum, hProcess);
}

uintptr_t Cheats::getLocalPlayer(HANDLE hProcess, uintptr_t moduleBase)
{
	return RPM<uintptr_t>(moduleBase + dwLocalPlayer, hProcess);
}

uintptr_t Cheats::getPlayer(int index, HANDLE hProcess, uintptr_t moduleBase)
{
	return RPM<uintptr_t>(moduleBase + dwEntityList + index * 0x10, hProcess);
}

void Cheats::playerShoot(HANDLE hProcess, uintptr_t moduleBase, bool switchWeapon, int fireTime, int pauseTime)
{
	if (switchWeapon == true) {
		WPM<int>(moduleBase + dwForceAttack, 0x5, hProcess);
		Sleep(fireTime);
		WPM<int>(moduleBase + dwForceAttack, 0x4, hProcess);
		Sleep(15);
		simulateKeyPress(VK_NUMPAD3);
		Sleep(20);
		simulateKeyPress(VK_NUMPAD1);
		Sleep(pauseTime);
	} else if (switchWeapon == false) 
	{
		WPM<int>(moduleBase + dwForceAttack, 0x5, hProcess);
		Sleep(fireTime);
		WPM<int>(moduleBase + dwForceAttack, 0x4, hProcess);
		Sleep(pauseTime);
	}
}

void Cheats::playerForceJump(HANDLE hProcess, uintptr_t moduleBase)
{
	WPM<int>(moduleBase + dwForceJump, 0x5, hProcess);
	Sleep(1);
	WPM<int>(moduleBase + dwForceJump, 0x4, hProcess);
}

int Cheats::getPlayerHealth(uintptr_t player, HANDLE hProcess)
{
	return RPM<int>(player + m_iHealth, hProcess);
}

Vector3 Cheats::getPlayerLocation(uintptr_t player, HANDLE hProcess)
{
	return RPM<Vector3>(player + m_vecOrigin, hProcess);
}

bool Cheats::getPlayerDormant(uintptr_t player, HANDLE hProcess)
{
	return RPM<int>(player + m_bDormant, hProcess);
}

bool Cheats::getPlayerInCrosshair(uintptr_t player, HANDLE hProcess, uintptr_t moduleBase)
{
	int CrosshairId;
	CrosshairId = RPM<int>(player + m_iCrosshairId, hProcess);
	int Entity = CrosshairId;
	int localTeam = getPlayerTeam(getLocalPlayer(hProcess, moduleBase), hProcess);
	int EnmTeam = getPlayerTeam(getPlayer(Entity, hProcess, moduleBase), hProcess);

	if (CrosshairId > 0) {
		if (localTeam == EnmTeam) {
			return true;
		}
		return true;
	}
	else {
		return false;
	}
}

Vector3 Cheats::getPlayerHead(uintptr_t player, HANDLE hProcess)
{
	struct boneMatrix_t { //make a struct for the bone matrix XYZ and pad it
		byte pad3[12];
		float x;
		byte pad1[12];
		float y;
		byte pad2[12];
		float z;
	};
	uintptr_t boneBase = RPM<uintptr_t>(player + m_dwBoneMatrix, hProcess);
	boneMatrix_t boneMatrix = RPM<boneMatrix_t>(boneBase + (sizeof(boneMatrix) * 8 /* 8 is the boneId for head*/), hProcess);
	return Vector3(boneMatrix.x, boneMatrix.y, boneMatrix.z);
}

int Cheats::getPlayerFlags(HANDLE hProcess, uintptr_t moduleBase)
{
	int fFlags = RPM<byte>(getLocalPlayer(hProcess, moduleBase) + m_fFlags, hProcess);
	return fFlags;
}

Vector3 WorldToScreen(const Vector3 pos, view_matrix_t matrix)
{
	struct Vector3 out;
	float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
	float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
	out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	out.x = SCREEN_WIDTH * .5f;
	out.y = SCREEN_HEIGTH * .5f;

	out.x += 0.5f * _x * SCREEN_WIDTH + 0.5f;
	out.y -= 0.5f * _y * SCREEN_HEIGTH + 0.5f;

	return out;
}



Vector3 Cheats::CreateStartPoint(const struct Vector3 playerHeadPosition, int randomHeight)
{
	struct Vector3 legitPlayerHeadPosition;
	legitPlayerHeadPosition.z = playerHeadPosition.z;
	float playerDistanze = playerHeadPosition.z / 10;
	legitPlayerHeadPosition.x = playerHeadPosition.x;
	// max +-800
	legitPlayerHeadPosition.y = playerHeadPosition.y;
	legitPlayerHeadPosition.y = legitPlayerHeadPosition.y + randomHeight / playerDistanze;


	return legitPlayerHeadPosition;
}

float Cheats::pythag(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int Cheats::randomNumber(int maxNumber, int minNumber)
{
	int result;
	int range = maxNumber - minNumber;
	result = rand() % range + minNumber;

	return result;
}


//puplic
int Cheats::FindClosestEnemy(HANDLE hProcess, uintptr_t moduleBase)
{
	vm = RPM<view_matrix_t>(moduleBase + dwViewMatrix, hProcess);
	float Finish;
	int ClosestEntity = 1;
	Vector3 Calc = { 0, 0, 0 };
	float Closest = FLT_MAX;
	int localTeam = getPlayerTeam(getLocalPlayer(hProcess, moduleBase), hProcess);
	for (int i = 1; i < 64; i++) { //Loops through all the entitys in the index 1-64.
		DWORD Entity = getPlayer(i, hProcess, moduleBase);
		int EnmTeam = getPlayerTeam(Entity, hProcess); if (EnmTeam == localTeam) continue;
		int EnmHealth = getPlayerHealth(Entity, hProcess); if (EnmHealth < 1 || EnmHealth > 100) continue;
		int Dormant = getPlayerDormant(Entity, hProcess); if (Dormant) continue;
		Vector3 headBone = WorldToScreen(getPlayerHead(Entity, hProcess), vm);
		
		Finish = pythag(headBone.x, headBone.y, xhairx, xhairy);
		if (Finish < Closest) {
			Closest = Finish;
			ClosestEntity = i;
		}
	}

	return ClosestEntity;
}

void Cheats::bHop(HANDLE hProcess, uintptr_t moduleBase)
{
	if (GetAsyncKeyState(VK_SPACE) && getPlayerFlags(hProcess, moduleBase) == 1) {
		
		playerForceJump(hProcess, moduleBase);
	}
}

void Cheats::radar(HANDLE hProcess, uintptr_t moduleBase)
{
	for (int i = 1; i < 64; i++) {
		if (getPlayer(i, hProcess, moduleBase)) {
			WPM<bool>(getPlayer(i, hProcess, moduleBase) + m_bSpotted, true, hProcess);
		}
	}
}

void Cheats::triggerbot(HANDLE hProcess, uintptr_t moduleBase, int VK_BUTTON, int timeToShoot, bool switchWeapon, int shootTime, int pauseTime)
{
	if (GetAsyncKeyState(VK_BUTTON)) {
		if (getPlayerInCrosshair(getLocalPlayer(hProcess, moduleBase), hProcess, moduleBase)) {
			Sleep(timeToShoot);
			playerShoot(hProcess, moduleBase, switchWeapon, shootTime, pauseTime);
		}
	}
}

void Cheats::aimbot(HANDLE hProcess, int closest, uintptr_t moduleBase, int VK_KEY, int aimbotFOV, float aimbotSmoothing)
{
	vm = RPM<view_matrix_t>(moduleBase + dwViewMatrix, hProcess);
	Vector3 closestw2shead = WorldToScreen(getPlayerHead(getPlayer(closest, hProcess, moduleBase), hProcess), vm);
	float dist = pythag(xhairx, xhairy, closestw2shead.x, closestw2shead.y);
	if (GetAsyncKeyState(VK_KEY) && closestw2shead.z >= 0.001f && dist < aimbotFOV) {
		SetCursorPos(closestw2shead.x, closestw2shead.y);
	}
}

void Cheats::sniperAutoChangeWeapon(HANDLE hProcess, uintptr_t moduleBase)
{
	if (getPlayerShot(hProcess, moduleBase))
	{
		Sleep(20);
		simulateKeyPress(VK_NUMPAD3);
		Sleep(10);
		simulateKeyPress(VK_NUMPAD1);
	}

}

Vector3 Cheats::ESP(HANDLE hProcess, uintptr_t moduleBase)
{

	vm = RPM<view_matrix_t>(moduleBase + dwViewMatrix, hProcess);
	int localTeam = getPlayerTeam(getLocalPlayer(hProcess, moduleBase), hProcess);

	for (int i = 1; i < 64; i++) {
		DWORD Entity = getPlayer(i, hProcess, moduleBase);

		int EnmTeam = getPlayerTeam(Entity, hProcess); if (EnmTeam == localTeam) continue;
		int EnmHealth = getPlayerHealth(Entity, hProcess); if (EnmHealth < 1 || EnmHealth > 100) continue;
		int Dormant = getPlayerDormant(Entity, hProcess); if (Dormant) continue;

		struct Vector3 Player = getPlayerLocation(Entity, hProcess);
		struct Vector3 PlayerHead;
		PlayerHead.x = Player.x;
		PlayerHead.y = Player.y;
		PlayerHead.z = Player.z + 75.f;
		struct Vector3  PlayerScreen = WorldToScreen(Player, vm);
		struct Vector3 PlayerHeadScreen = WorldToScreen(PlayerHead, vm);
		float height = PlayerHeadScreen.y - PlayerScreen.y;
		float width = height / 2.4f;
		
		if (PlayerScreen.z >= 0.1f)
		{
			return PlayerScreen;
		}
		/*
		if (PlayerScreen.z >= 0.1f) {
			espUI.DrawRectangle(PlayerScreen.x - (width / 2), PlayerScreen.y, width, height, 4, EnemyBrush, hdc);
			espUI.DrawLine(xhairx, xhairy, PlayerScreen.x, PlayerScreen.y, 3, 0x0000ff, hdc);
		}
		*/
	}
}


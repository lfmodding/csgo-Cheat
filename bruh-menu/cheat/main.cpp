#include "gui.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cstdlib>


#include "cheats.h"
#include "memoryManager.h"


Cheats cheats;

using namespace std;

HWND hwnd;
HWND consoleHWND;
DWORD procId;
HANDLE hProcess;
uintptr_t moduleBase;
int closest;


void getClosestEnemyThread()
{
	while (true)
	{
		closest = cheats.FindClosestEnemy(hProcess, moduleBase);
	}

}
void mainCheatThread() 
{
	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive - Direct3D 9");
	GetWindowThreadProcessId(hwnd, &procId);
	moduleBase = memoryManager::GetModuleAddress("client.dll", procId);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	while (true) 
	{
		gui::cheatsHandler(closest, hProcess, moduleBase);
		Sleep(1);
	}
}
void TriggerbotThread()
{
	while (true)
	{
		gui::triggerbotHandler(hProcess, moduleBase);
		Sleep(1);
	}

}
void aimbotThread()
{
	while (true)
	{
		gui::aimbotHandler(closest, hProcess, moduleBase);
		Sleep(1);
	}

}




int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)getClosestEnemyThread, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainCheatThread, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TriggerbotThread, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)aimbotThread, NULL, NULL, NULL);

	// create gui
	gui::CreateHWindow("Cheat Menu", "Cheat Menu Class");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::exit)
	{
		gui::BeginRender();
		gui::Render(hProcess, moduleBase);
		gui::EndRender();

		Sleep(10);
	}

	//destroy gui
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}

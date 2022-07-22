#pragma once
#include <d3d9.h>

namespace gui
{
	constexpr int WIDTH = 500;
	constexpr int HEIGHT = 300;

	inline bool exit = true;

	//winapi window vars
	inline HWND window = nullptr;
	inline WNDCLASSEXA windowClass = { };

	// window position points
	inline POINTS position = { };

	// direct x state vars
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = { };

	//creation/destruction of window
	void CreateHWindow(
		const char* windowName,
		const char* className) noexcept;
	void DestroyHWindow() noexcept;

	//creation/destruction of d3d device
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	//creation/destruction of ImGui
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render(HANDLE hProcess, uintptr_t moduleBase) noexcept;

	void cheatsHandler(int closestEnemy, HANDLE hProcess, uintptr_t moduleBase);

	void aimbotHandler(int closestEnemy, HANDLE hProcess, uintptr_t moduleBase);

	void triggerbotHandler(HANDLE hProcess, uintptr_t moduleBase);

}
#include "gui.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"


#include "cheats.h"

Cheats guicheat;
bool bunnyHopCheat = false;
bool aimbotCheat = false;
bool triggerbotCheat = false;
bool radarCheat = false;
bool sniperAutoChangeWeaponCheat = false;

bool triggerbotPresets = true;
static int triggerbotShotDelay = 1;
static int triggerbotShootTime = 150;
static int triggerbotPauseTime = 250;

int aimbotFOV = 40;
float aimbotSmoothing = 0.025f;

const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN); const int xhairx = SCREEN_WIDTH / 2;
const int SCREEN_HEIGTH = GetSystemMetrics(SM_CYSCREEN); const int xhairy = SCREEN_HEIGTH / 2;


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;
		
	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // disable alt application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			auto rect = ::RECT{ };
			const auto points = MAKEPOINTS(longParameter);
			

			GetWindowRect(gui::window, &rect);
			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}
	
	return DefWindowProcW(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(
	const char* windowName,
	const char* className) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA(
		className,
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}


bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	
	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}


void gui::Render(HANDLE hProcess, uintptr_t moduleBase) noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"csgo Cheat",
		&exit,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);
	ImGui::BeginTabBar("mainTabBar"); 

	//shots
	if (ImGui::BeginTabItem("shots")) {

		ImGui::Checkbox("Triggerbot", &triggerbotCheat);
		if (triggerbotCheat) {
			if (ImGui::Button("Presets"))
				triggerbotPresets = true;
			ImGui::SameLine();
			if (ImGui::Button("Custom"))
				triggerbotPresets = false;

			ImGui::Separator();
			ImGui::Spacing();

			if (triggerbotPresets) {
				if (ImGui::Button("Sniper")) {
					triggerbotShotDelay = 10;
					triggerbotShootTime = 50;
					triggerbotPauseTime = 50;
				}
				ImGui::SameLine();
				if (ImGui::Button("Longrange")) {
					triggerbotShotDelay = 20;
					triggerbotShootTime = 200;
					triggerbotPauseTime = 240;
				}
				ImGui::SameLine();
				if (ImGui::Button("Closerange")) {
					triggerbotShotDelay = 7;
					triggerbotShootTime = 230;
					triggerbotPauseTime = 220;
				}

				
			}

			if (!triggerbotPresets) {
				ImGui::Text("triggerbot shot delay(ms): ");
				ImGui::SliderInt("##triggerbotdelaytime", &triggerbotShotDelay, 1, 200, "%d", ImGuiSliderFlags_AlwaysClamp);
				ImGui::Text("triggerbot burst time(ms): ");
				ImGui::SliderInt("##triggerbotshoottime", &triggerbotShootTime, 1, 400, "%d", ImGuiSliderFlags_AlwaysClamp);
				ImGui::Text("triggerbot delay to next Burst(ms): ");
				ImGui::SliderInt("##triggerbotpausetime", &triggerbotPauseTime, 1, 700, "%d", ImGuiSliderFlags_AlwaysClamp);
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}
		
		ImGui::Checkbox("Aimbot", &aimbotCheat);
		if (aimbotCheat)
		{
			ImGui::Text("aimbot FOV:");
			ImGui::SliderInt("##aimBotFOV", &aimbotFOV, 1, 1000);
			ImGui::Text("aimbot Smoothing:");
			ImGui::SliderFloat("", &aimbotSmoothing, 0.025f, 0.99f);
		}

		ImGui::EndTabItem();
	}

	//wallhacks
	if (ImGui::BeginTabItem("Walls")) {
		ImGui::Checkbox("Mark all Enemys On Radar", &radarCheat);
		ImGui::EndTabItem();
	}
	

	//miscs
	if (ImGui::BeginTabItem("miscs")) {
		ImGui::Checkbox("Bunny Hop", &bunnyHopCheat);
		ImGui::Checkbox("auto swap weapon after shot", &sniperAutoChangeWeaponCheat);
		ImGui::EndTabItem();
	}

	//Debug
	if (ImGui::BeginTabItem("Debug")) {
		
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
	ImGui::End();

	
}

//cheats Handler
void gui::cheatsHandler(int closestEnemy, HANDLE hProcess, uintptr_t moduleBase) 
{
	if (bunnyHopCheat)
	{
		guicheat.bHop(hProcess, moduleBase);
	}

	if (radarCheat)
	{
		guicheat.radar(hProcess, moduleBase);
	}
}
void gui::aimbotHandler(int closestEnemy, HANDLE hProcess, uintptr_t moduleBase)
{
	if (aimbotCheat)
	{
		guicheat.aimbot(hProcess, closestEnemy, moduleBase, VK_XBUTTON2, aimbotFOV, aimbotSmoothing);
	}
}
void gui::triggerbotHandler(HANDLE hProcess, uintptr_t moduleBase)
{
	if (triggerbotCheat)
	{
		guicheat.triggerbot(hProcess, moduleBase, VK_XBUTTON1, triggerbotShotDelay, sniperAutoChangeWeaponCheat, triggerbotShootTime, triggerbotPauseTime);
	}
	if (sniperAutoChangeWeaponCheat)
	{
		guicheat.sniperAutoChangeWeapon(hProcess, moduleBase);
	}
}

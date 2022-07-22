#pragma once
#include <Windows.h>

class Vector3 {
public:
	float x, y, z;
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};


namespace memoryManager
{
	uintptr_t GetModuleAddress(const char* modName, DWORD procId);
	
	template<typename T> 
	T RPM(SIZE_T address, HANDLE hProcess) 
	{
		T buffer;
		ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
		return buffer;
	}
	template<typename T> 
	void WPM(SIZE_T address, T buffer, HANDLE hProcess)
	{
		WriteProcessMemory(hProcess, (void*)address, &buffer, sizeof(T), nullptr);
	}
}


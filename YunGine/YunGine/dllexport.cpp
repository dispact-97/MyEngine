#include <iostream>
#include <windows.h>
#include "dllexport.h"

typedef void (*Initialize)(HWND);

DllLoad::DllLoad()
	: m_hModule(nullptr)
{

}

DllLoad::~DllLoad()
{

}

void DllLoad::DllGet(HWND hwnd)
{
	HMODULE m_hModule = LoadLibrary(L"DXRenderer.dll");
	if (m_hModule == nullptr)
	{
		std::cout << "Failed to load DLL" << std::endl;
		return;
	}

	typedef void (*DllGetFunc)(HWND);
	DllGetFunc dllGetFunc = reinterpret_cast<DllGetFunc>(GetProcAddress(m_hModule, "Initialize"));
	if (dllGetFunc == nullptr)
	{
		std::cout << "Failed to get function address" << std::endl;
		FreeLibrary(m_hModule);
		return;
	}

	dllGetFunc(hwnd);
}

void DllLoad::DllFree(HWND hwnd)
{
	FreeLibrary(m_hModule);
}

// void DllLoad::DllGet(HWND hwnd)
// {
// 	HMODULE hModule = LoadLibrary(L"DXRenderer.dll");
// 	if (hModule == nullptr)
// 	{
// 		std::cout << "Failed to load DLL" << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "Success to load DLL" << std::endl;
// 		Initialize InitFunc = reinterpret_cast<Initialize>(GetProcAddress(hModule, "Initialize"));
// 		if (InitFunc != nullptr)
// 		{
// 			// DLL 초기화 함수 호출
// 			InitFunc(hwnd);
// 		}
// 		else
// 		{
// 			std::cout << "Failed to get initialization function" << std::endl;
// 		}
// 	}
// 
// }

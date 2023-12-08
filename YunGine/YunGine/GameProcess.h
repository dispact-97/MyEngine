#pragma once
#include <windows.h>
#include <memory>
#include "..\\DXRenderer\\IDX11Render.h"
#include "GameTimer.h"

#ifdef _DEBUG
#define RENDER_PATH L"..\\x64\\Debug\\DXRenderer.dll"
#else
#define RENDER_PATH L"DXRenderer.dll"
#endif // _DEBUG

/// <summary>
/// 수업시간에 만든 게임 엔진의 최외곽 부분
/// 
/// 길잃은 어린양을 구원해주세요
/// 
/// 23.06.16 -Dispact-
/// </summary>

class GameProcess
{
public:
	GameProcess() = default;
	~GameProcess();

private:
	// 공통적인 설정

	// 화면의 크기
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;

	// 윈도 관련
	HWND hWnd;
	MSG m_Msg;
	
	// 그래픽스 엔진
	HMODULE m_hModule;
	std::unique_ptr<IDX11Render> renderer;
	
	// 게임 타이머
	GameTimer* m_pTimer;

public:
	// 게임용 윈도 프레임웍을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);

	// 메인 게임 루프. 실제 게임이 진행되는 반복구문
	void Loop();

	void Update();
	void Render();
	
	HRESULT CreateGraphicEngine();
	HRESULT DeleteGraphicEngine();

	// 게임을 정리한다.
	void Finalize();

};


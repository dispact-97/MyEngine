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
/// �����ð��� ���� ���� ������ �ֿܰ� �κ�
/// 
/// ������ ����� �������ּ���
/// 
/// 23.06.16 -Dispact-
/// </summary>

class GameProcess
{
public:
	GameProcess() = default;
	~GameProcess();

private:
	// �������� ����

	// ȭ���� ũ��
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;

	// ���� ����
	HWND hWnd;
	MSG m_Msg;
	
	// �׷��Ƚ� ����
	HMODULE m_hModule;
	std::unique_ptr<IDX11Render> renderer;
	
	// ���� Ÿ�̸�
	GameTimer* m_pTimer;

public:
	// ���ӿ� ���� �����ӿ��� �ʱ�ȭ�Ѵ�.
	HRESULT Initialize(HINSTANCE hInstance);

	// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
	void Loop();

	void Update();
	void Render();
	
	HRESULT CreateGraphicEngine();
	HRESULT DeleteGraphicEngine();

	// ������ �����Ѵ�.
	void Finalize();

};


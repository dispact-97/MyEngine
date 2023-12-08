#include <windows.h>
#include "GameProcess.h"

/// <summary>
/// ���� ����
/// ���α׷� �������� ����
/// 
/// ������ � ���� �������ּ���
/// 
/// 2023.06.16 -Dispact-
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nShowCmd"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
	{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ���� ���� Ŭ����
// ���ο��� ���� ������ �޽��� �ݹ��� ó���Ѵ�.
	// ���� �׷��Ƚ� ���� ���� �����Ѵ�.
	GameProcess* pGameProcess = new GameProcess();
	pGameProcess->Initialize(hInstance);
	pGameProcess->Loop();
	pGameProcess->Finalize();

	delete pGameProcess;

	return 0; // (int)msg.wParam;

	}
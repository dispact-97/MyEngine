#include <windows.h>
#include "GameProcess.h"

/// <summary>
/// 엔진 기초
/// 프로그램 진입지점 생성
/// 
/// 길잃은 어린 양을 구원해주세요
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

	// 게임 진행 클래스
// 내부에서 윈도 생성과 메시지 콜백을 처리한다.
	// 또한 그래픽스 엔진 등을 포함한다.
	GameProcess* pGameProcess = new GameProcess();
	pGameProcess->Initialize(hInstance);
	pGameProcess->Loop();
	pGameProcess->Finalize();

	delete pGameProcess;

	return 0; // (int)msg.wParam;

	}
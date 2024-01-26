#pragma once
#include "IDX11Render.h"

using namespace std;
/// <summary>
/// 차근차근 천천히 dll화 시켜보자
/// 23.06.20
/// </summary>

// DirectX 11에서 3D그래픽을 설정하고 그리기 위한 모든 D3D기능이 포함되어 있다.
#pragma comment(lib, "d3d11.lib")
// 모니터의 재생을 위한 라이브러리
#pragma comment(lib, "dxgi.lib")
// 셰이더 컴파일링 기능
#pragma comment(lib, "d3dcompiler.lib") 
// DXTk
#pragma  comment(lib, "DirectXTK.lib")


/// 릴리즈 모드에서 RuntimeLibrary에 대해 불일치가 검색되었습니다. 에러가 발생중
/// -23.07.19- 인재원에서
///  #pragma comment를 사용해서 파일을 쓰겠다고 했다면 속성에서 종속성을 풀어줘야 한다. 에러 해결
/// -23.07.21- 인재원에서

#include "Vertex.h"

//스마트 포인터를 사용하기 위한 헤더
#include <wrl\client.h>

// 어댑터를 위한 헤더
// 상위헤더 참조시 하위헤더는 자동으로 포함된다
#include <d3d11_4.h>
#include <dxgi1_2.h>

#include <vector>

class Axis;
class Cube;
class Grid;
class Camera;
class Font;
class MouseClass;
class RenderableBase;
class InputClass;
class IngameImage;

//test
class NewCube;


class DX11Render : public IDX11Render
{
public:
	// 싱글톤으로 하는게 좋을까?
	DX11Render();
	virtual ~DX11Render();

	/// <summary>
	/// dll로 만들면 이 함수들을 모두 하나로 묶어서 사용해야하나?
	/// __declspec(dllexport)func(int a, int b)
	/// -23.06.20 -kocca, Dispact-
	/// </summary>
	/// 
	// bool값으로 만들어서 return false를 반환해야한다.
	virtual long Initialize() override;	// 그래픽 엔진 초기화

	virtual void Update(float deltaTime,float fps, float mspf) override;
	// 움직임 테스트

	virtual void Render() override;
	virtual void BeginRender(float red, float green, float blue, float alpha) override;
	virtual void DrawObject() override;
	virtual void EndRender() override;

	HRESULT InitVB();

	virtual void Finalize() override;

	// 윈도우 중앙
	// const int _windowCenterX = _windowWidth / 2;
	// const int _windowCenterY = _windowHeight / 2;

private:
	// void OnResize(); 
	// Message쪽에서 사이즈가 재정의 됬을때 호출하는 함수이지만 일단 보류
	// Render 함수 내에서 사용할 것들

	HRESULT CreateHandleWindow();
	HRESULT CreateDevice();
	HRESULT CreateSwapChain(HWND hWnd);
	HRESULT CreateBackBuffer();
	HRESULT CreateRaster();

	// 오브젝트들 생성하기
	HRESULT CreateObject();
	HRESULT CreateFont();
	HRESULT CreateInput(HWND hWnd,int width,int height);
	HRESULT CreateCamera();
	HRESULT CreateCube();
	HRESULT CreateGrid();
	HRESULT CreateAxis();
	HRESULT CreateUI();

	// 윈도우 생성
	HWND hWnd;
	HINSTANCE hInstance;

	// 윈도우 가로,세로
	const int _windowWidth = 1600;
	const int _windowHeight = 1080;

	// 시간 변수
	float m_deltaTime;
	float m_fps = 0.0f;
	float m_mspf = 0.0f;

	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 디버그 텍스트
	void RenderAllText();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_p3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_p3DDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;

	// 뎁스 스탠실
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	// 레스터 상태
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pCurrRasterState;	// 현재 레스터
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pSolidRasterState;	// 채우는 레스터
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pWireRasterState;	// 와이어 레스터-> 선으로 나타낸다는뜻

	// 스왑체인을 만들기 위한 변수
	Microsoft::WRL::ComPtr<IDXGISwapChain> sm_SwapChain;
	Microsoft::WRL::ComPtr<IDXGIDevice3> sm_dxgiDevice;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>m_pBackBuffer;

	// Direct2D와 같은 다른 Factory에서 사용할 DXGI 장치 개체를 만듭니다.
	Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;

	// 매트릭스
	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;

	// 개별 클래스
	std::vector<RenderableBase*> objectVector;
	Axis* m_pAxis;
	Grid* m_pGrid;
	Cube* m_pCube;
	//테스팅
	NewCube* m_pNewCube;
	Cube* m_pSecCube;
	Camera* m_pCamera;
	Font* m_pFont;
	InputClass* m_pInput;
	IngameImage* m_IngameImage;

	// 마우스
	MouseClass* m_pMouse;
	struct mousePosition
	{
		int mousePosX = 0;
		int mousePosY = 0;
	};
	mousePosition Curr;
	mousePosition Now;
	bool ignoreMouseMove = false;
	// TAB으로 카메라랑 오브젝트 바꾸기

	bool switchObejct = true;

	// 버퍼
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

};


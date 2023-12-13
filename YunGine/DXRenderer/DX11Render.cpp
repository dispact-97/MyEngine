#include "DX11Render.h"
#include <iostream>
#include <fstream>
#include <string>

#include <d3dcompiler.h>
#include <d3dcommon.h>

#include "Utility.h"

#include "RenderableBase.h"
#include "Axis.h"
#include "Cube.h"
#include "Grid.h"
#include "Camera.h"
#include "Font.h"
#include "MouseClass.h"

static int static_mouseXpos = 0;
static int static_mouseYpos = 0;
static int static_windowWidth = 0;
static int static_windowHeight = 0;

// dll로 부를때 랜더러를 만드는 함수의 주소를 가지고 있는다.
// return을 포인터로 받아줄 수 있다.
IDX11Render* CreateRenderer()
{
	return new DX11Render();
}

// dll로 랜더러를 지우는 함수
// 함수의 주소만 삭제하면 된다.
void DeleteRenderer(IDX11Render* instance)
{
	delete instance;
}

DX11Render::DX11Render()
	: VertexBuffer(nullptr),
	IndexBuffer(nullptr),
	hInstance(),
	hWnd(),
	m_deltaTime(),
	m_pCamera(nullptr),
	m_pGrid(nullptr),
	m_pAxis(nullptr),
	m_pCube(nullptr),
	m_pFont(nullptr),
	m_pMouse(nullptr),
	m_WorldMatrix
	{ 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f },
	m_ViewMatrix(),
	m_ProjectionMatrix()
{

}

DX11Render::~DX11Render()
{
	Curr.mousePosX = 0;
	Curr.mousePosY = 0;
}

long DX11Render::Initialize()
{
	HRESULT hr = S_OK;

	hr = CreateHandleWindow();
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateDevice();
	if (FAILED(hr))
	{
		return false;
		// 장치 인터페이스 생성 실패가 발생할 경우 이를 처리합니다.
		// 예를 들어, 기능 수준 요구 사항을 줄이거나 페일오버 
		// 워프 렌더링.
	}

	hr = CreateSwapChain(hWnd);	// 스왑체인 생성
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateBackBuffer();
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateRaster();
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateObject();	// 오브젝트들을 모두 여기서 만든다.
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void DX11Render::Update(float deltaTime, float fps, float mspf)
{
	m_deltaTime = deltaTime;
	m_fps = fps;
	m_mspf = mspf;

	Now.mousePosX = static_mouseXpos;
	Now.mousePosY = static_mouseYpos;

	// 카메라
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pCamera->Walk(10.0f * deltaTime);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pCamera->Walk(-10.0f * deltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pCamera->Strafe(-10.0f * deltaTime);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pCamera->Strafe(10.0f * deltaTime);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_pCamera->WorldUpDown(-10.0f * deltaTime);
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_pCamera->WorldUpDown(10.0f * deltaTime);
	}

	// 카메라 회전
	m_pCamera->RotateX(0.3f * deltaTime * (Now.mousePosY - Curr.mousePosY));
	m_pCamera->RotateY(0.3f * deltaTime * (Now.mousePosX - Curr.mousePosX));

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pCamera->RotateY(3.0f * deltaTime);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pCamera->RotateY(-3.0f * deltaTime);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pCamera->RotateX(-3.0f * deltaTime);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pCamera->RotateX(3.0f * deltaTime);
	}

	m_pCamera->UpdateViewMatrix();

	for (auto& iter : objectVector)
	{
		iter->ObjectUpdate(DirectX::XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());
	}

	Curr.mousePosX = Now.mousePosX;
	Curr.mousePosY = Now.mousePosY;
}

void DX11Render::Render()
{
	// 이 안에다가 비긴렌더, 드로우오브젝트, 엔드렌더를 넣는게 효율적인가?
	BeginRender(0, 0, 0, 1);
	DrawObject();
	EndRender();
}

void DX11Render::BeginRender(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;	// r
	color[1] = green;	// g
	color[2] = blue;	// b
	color[3] = alpha;	// a

	m_p3DDeviceContext->OMSetRenderTargets(
		1,
		m_pRenderTargetView.GetAddressOf(),
		m_pDepthStencilView.Get()
	);

	// Clear the back buffer.
	m_p3DDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	// Clear the depth buffer.
	m_p3DDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

}

void DX11Render::DrawObject()
{
	DirectX::XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	DirectX::XMMATRIX viewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	DirectX::XMMATRIX projMatrix = XMLoadFloat4x4(&m_ProjectionMatrix);

	m_p3DDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	RenderAllText();

	for (auto& iter : objectVector)
	{
		iter->Render();
	}

	// 레스터라이저 상태 설정 
	m_p3DDeviceContext->RSSetState(0);
}

void DX11Render::EndRender()
{
	sm_SwapChain->Present(1, 0);

	return;
}

HRESULT DX11Render::InitVB()
{
	return S_OK;
}

void DX11Render::Finalize()
{
	if (sm_SwapChain)
	{
		sm_SwapChain->Release();
	}

	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
	}

	if (m_p3DDevice)
	{
		m_p3DDevice->Release();
	}
}

HRESULT DX11Render::CreateHandleWindow()
{
	HRESULT hr = S_OK;
	/// Win32 관련
	// 윈도 클래스
	// 
	// 멀티바이트에서 유니코드로 넘어오면서 char* 에러가 났는데
	// 이런식으로 형변환을 해도 될까?
	wchar_t szAppName[] = L"YJD3Ddemo Engine";
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 윈도 생성
	hWnd = CreateWindow(
		// 멀티바이트에서 유니코드로 넘어오면서 char* 에러가 났는데
		// 이런식으로 형변환을 해도 될까?
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		100, 100, _windowWidth, _windowHeight,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return S_FALSE;

	// 생성된 윈도를 화면에 표시
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	static_windowWidth = _windowWidth;
	static_windowHeight = _windowHeight;

	return hr;
}

HRESULT DX11Render::CreateDevice()
{
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_1;

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(
		nullptr,                    // 기본 어댑터를 사용하려면 nullptr을 지정하십시오.
		D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 그래픽 드라이버를 사용하여 장치를 생성합니다.
		nullptr,                       // 드라이버가 D3D_DRIVER_TYPE_SOFTWARE가 아닌 한 0이어야 합니다.
		deviceFlags,                // 디버그 및 Direct2D 호환성 플래그를 설정합니다.
		levels,                     // 이 앱이 지원할 수 있는 기능 수준 목록입니다.
		ARRAYSIZE(levels),          // 위 목록의 크기입니다.
		D3D11_SDK_VERSION,          // Windows Store 앱의 경우 항상 D3D11_SDK_VERSION으로 설정하십시오.
		&m_p3DDevice,   // 생성된 Direct3D 장치를 반환합니다.
		&FeatureLevels,             // 생성된 디바이스의 피쳐 수준을 반환합니다.
		&m_p3DDeviceContext			        // 장치 즉시 컨텍스트를 반환합니다.
	);

	return hr;
}

HRESULT DX11Render::CreateSwapChain(HWND hWnd)
{
	HRESULT hr = S_OK;

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;      //multisampling setting
	desc.SampleDesc.Quality = 0;    //vendor-specific flag
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = hWnd;

	m_p3DDevice.As(&dxgiDevice);

	hr = dxgiDevice->GetAdapter(&adapter);

	if (SUCCEEDED(hr))
	{
		adapter->GetParent(IID_PPV_ARGS(&factory));

		hr = factory->CreateSwapChain(
			m_p3DDevice.Get(),
			&desc,
			&sm_SwapChain
		);
	}

	return hr;
}

HRESULT DX11Render::CreateBackBuffer()
{
	HRESULT hr;

	// 백버퍼 만들기
	//====================================
	D3D11_TEXTURE2D_DESC m_BackBufferDesc;

	hr = sm_SwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)(m_pBackBuffer.GetAddressOf()));

	hr = m_p3DDevice->CreateRenderTargetView(
		m_pBackBuffer.Get(),
		nullptr,
		m_pRenderTargetView.GetAddressOf()
	);

	m_pBackBuffer->GetDesc(&m_BackBufferDesc);	// Desc 서술자? -> 백버퍼에대한 설정값이 들어있는 구조체


	// 뎁스 스탠실
	// 3D를 띄우기 시작할때 필요함
	//======================================

	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (m_BackBufferDesc.Width),
		static_cast<UINT> (m_BackBufferDesc.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	m_p3DDevice->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&m_pTexture2D
	);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	m_p3DDevice->CreateDepthStencilView(
		m_pTexture2D.Get(),
		&depthStencilViewDesc,
		&m_pDepthStencilView
	);

	//뷰포트
	//=================================================

	D3D11_VIEWPORT m_ViewPort;
	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
	m_ViewPort.Height = (float)m_BackBufferDesc.Height;
	m_ViewPort.Width = (float)m_BackBufferDesc.Width;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;

	m_p3DDeviceContext->RSSetViewports(
		1,
		&m_ViewPort
	);

	return hr;
}

HRESULT DX11Render::CreateRaster()
{
	HRESULT hr = S_OK;

	// 레스터
	//=================================================

	D3D11_RASTERIZER_DESC solidRasterDesc;	// 채우는 모드
	ZeroMemory(&solidRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidRasterDesc.FillMode = D3D11_FILL_SOLID;	// 채우기 모드
	solidRasterDesc.CullMode = D3D11_CULL_BACK;		// Back은 그리지 않음
	solidRasterDesc.FrontCounterClockwise = false;	// 반시게를 false = 시계방향으로 그리겠다는 뜻이다.
	solidRasterDesc.DepthClipEnable = true;			// 거리에 따라 클리핑을 할지

	hr = m_p3DDevice->CreateRasterizerState
	(
		&solidRasterDesc,
		m_pSolidRasterState.GetAddressOf()
	);


	//=================================================

	D3D11_RASTERIZER_DESC wireRasterDesc;	// 채우지않음-> 선을그림
	ZeroMemory(&wireRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireRasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireRasterDesc.CullMode = D3D11_CULL_NONE;
	wireRasterDesc.FrontCounterClockwise = false;
	wireRasterDesc.DepthClipEnable = true;

	hr = m_p3DDevice->CreateRasterizerState
	(
		&wireRasterDesc,
		m_pWireRasterState.GetAddressOf()
	);

	//====================================================

	return hr;
}

HRESULT DX11Render::CreateObject()
{
	HRESULT hr = S_OK;

	m_pMouse = new MouseClass();

	hr = CreateFont();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateCamera();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateAxis();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateGrid();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateCube();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT DX11Render::CreateFont()
{
	HRESULT hr = S_OK;

	m_pFont = new Font(m_p3DDevice.Get(), m_p3DDeviceContext.Get());
	if (!m_pFont)
	{
		return hr = S_FALSE;
	}

	return hr;

}

HRESULT DX11Render::CreateCamera()
{
	HRESULT hr = S_OK;

	m_pCamera = new Camera();

	// 카메라를 만들고
	// 세팅을 해준다.
	m_pCamera->SetLens(0.25f * 3.1415926535f, 1280.0f / 720.0f, 1.0f, 1000.0f);

	// LH(Left Hand)방향으로
	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 1.0f, 1000.0f);
	DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, p);

	// 맨처음에 보는 카메라의 포지션, 쳐다보는 방향,UP벡터 정하기
	m_pCamera->LookAt(DirectX::XMFLOAT3(8.0f, 8.0f, -8.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1.0f, 0));

	return S_OK;
}

HRESULT DX11Render::CreateCube()
{
	HRESULT hr = S_OK;

	m_pCube = new Cube(m_p3DDevice, m_p3DDeviceContext, m_pWireRasterState);
	if (!m_pCube)
	{
		return hr = S_FALSE;
	}
	objectVector.push_back(m_pCube);

	return S_OK;
}

HRESULT DX11Render::CreateGrid()
{
	HRESULT hr = S_OK;

	m_pGrid = new Grid(m_p3DDevice, m_p3DDeviceContext, m_pWireRasterState);
	if (!m_pGrid)
	{
		return hr = S_FALSE;
	}
	objectVector.push_back(m_pGrid);

	return S_OK;
}

HRESULT DX11Render::CreateAxis()
{
	HRESULT hr = S_OK;

	m_pAxis = new Axis(m_p3DDevice, m_p3DDeviceContext, m_pWireRasterState);
	if (!m_pAxis)
	{

		return hr = S_FALSE;
	}
	objectVector.push_back(m_pAxis);

	return S_OK;
}

LRESULT CALLBACK DX11Render::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;

	// 윈도우 프로시저에서 객체 포인터 가져오기
	DX11Render* renderer = reinterpret_cast<DX11Render*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
		//case WM_PAINT:
		//{
		//	hdc = BeginPaint(hWnd, &ps);
		//	EndPaint(hWnd, &ps);
		//	break;
		//}

		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			static_mouseXpos = x;
			static_mouseYpos = y;

			if (x < 1 || y < 1 || x >1598 || y>1078)
			{
				SetCursorPos(static_windowWidth / 2, static_windowHeight / 2);
			}

			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void DX11Render::RenderAllText()
{
	m_pFont->RenderString("DeltaTime : ", 0.0f, 0.0f);
	m_pFont->RenderString(m_deltaTime, 110.0f, 0.0f);

	m_pFont->RenderString("FPS : ", 0.0f, 18.0f);
	m_pFont->RenderString(m_fps, 48.0f, 18.0f);

	m_pFont->RenderString("MousePosX : ", 0.0f, 36.0f);
	m_pFont->RenderString(static_mouseXpos, 120.0f, 36.0f);

	m_pFont->RenderString("MousePosY : ", 0.0f, 54.0f);
	m_pFont->RenderString(static_mouseYpos, 120.0f, 54.0f);
}

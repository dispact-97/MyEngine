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
#include "NewCube.h"
#include "Grid.h"
#include "Camera.h"
#include "Font.h"
#include "MouseClass.h"
#include "InputClass.h"
#include "IngameImage.h"

static int static_mouseXpos = 0;
static int static_mouseYpos = 0;
static int static_windowWidth = 0;
static int static_windowHeight = 0;

// dll�� �θ��� �������� ����� �Լ��� �ּҸ� ������ �ִ´�.
// return�� �����ͷ� �޾��� �� �ִ�.
IDX11Render* CreateRenderer()
{
	return new DX11Render();
}

// dll�� �������� ����� �Լ�
// �Լ��� �ּҸ� �����ϸ� �ȴ�.
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
	m_pNewCube(nullptr),
	m_pFont(nullptr),
	m_pMouse(nullptr),
	m_pInput(nullptr),
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
		// ��ġ �������̽� ���� ���а� �߻��� ��� �̸� ó���մϴ�.
		// ���� ���, ��� ���� �䱸 ������ ���̰ų� ���Ͽ��� 
		// ���� ������.
	}

	hr = CreateSwapChain(hWnd);	// ����ü�� ����
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

	hr = CreateObject();	// ������Ʈ���� ��� ���⼭ �����.
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

	m_pInput->Frame();	// Update���� � Ű�� �������� �޾ƿ�

	if (m_pInput->GetKeyDown(DIK_TAB) && switchObejct == true)
	{
		switchObejct = false;
	}
	else if (m_pInput->GetKeyDown(DIK_TAB) && switchObejct == false)
	{
		switchObejct = true;
	}

	// ī�޶�
	if (m_pInput->GetKey(DIK_W) && switchObejct == true)
	{
		m_pCamera->Walk(10.0f * deltaTime);
	}
	else if (m_pInput->GetKey(DIK_W) && switchObejct == false)
	{
		m_pNewCube->Move(0.0f, 0.0f, 10.0f * deltaTime);
	}

	if (m_pInput->GetKey(DIK_S) && switchObejct == true)
	{
		m_pCamera->Walk(-10.0f * deltaTime);
	}
	else if (m_pInput->GetKey(DIK_S) && switchObejct == false)
	{
		m_pNewCube->Move(0.0f, 0.0f, -10.0f * deltaTime);
	}

	if (m_pInput->GetKey(DIK_A) && switchObejct == true)
	{
		m_pCamera->Strafe(-10.0f * deltaTime);
	}
	else if (m_pInput->GetKey(DIK_A) && switchObejct == false)
	{
		m_pNewCube->Move(-10.0f * deltaTime, 0.0f, 0.0f);
	}

	if (m_pInput->GetKey(DIK_D) && switchObejct == true)
	{
		m_pCamera->Strafe(10.0f * deltaTime);
	}
	else if (m_pInput->GetKey(DIK_D) && switchObejct == false)
	{
		m_pNewCube->Move(10.0f * deltaTime, 0.0f, 0.0f);
	}

	if (m_pInput->GetKey(DIK_C) && switchObejct == true)
	{
		m_pCamera->WorldUpDown(-10.0f * deltaTime);
	}
	else if (m_pInput->GetKey(DIK_C) && switchObejct == false)
	{
		m_pNewCube->Move(0.0f, -10.0f * deltaTime, 0.0f);
	}

	if (m_pInput->GetKey(DIK_SPACE) && switchObejct == true)
	{
		m_pCamera->WorldUpDown(10.0f * deltaTime);
	}
	else if (m_pInput->GetKey(DIK_SPACE) && switchObejct == false)
	{
		m_pNewCube->Move(0.0f, 10.0f * deltaTime, 0.0f);
	}

	// ī�޶� ȸ��
	if (m_pInput->GetMouseDown(0))
	{
		m_pCamera->RotateX(0.3f * deltaTime * (Now.mousePosY - Curr.mousePosY));
		m_pCamera->RotateY(0.3f * deltaTime * (Now.mousePosX - Curr.mousePosX));
	}

	//if (GetAsyncKeyState(VK_RBUTTON)& 0x8000)
	//{
	//	m_pCamera->RotateX(0.3f * deltaTime * (Now.mousePosY - Curr.mousePosY));
	//	m_pCamera->RotateY(0.3f * deltaTime * (Now.mousePosX - Curr.mousePosX));
	//}

	if (m_pInput->GetKey(DIK_RIGHT))
	{
		m_pCamera->RotateY(3.0f * deltaTime);
	}

	if (m_pInput->GetKey(DIK_LEFT))
	{
		m_pCamera->RotateY(-3.0f * deltaTime);
	}

	if (m_pInput->GetKey(DIK_UP))
	{
		m_pCamera->RotateX(-3.0f * deltaTime);
	}

	if (m_pInput->GetKey(DIK_DOWN))
	{
		m_pCamera->RotateX(3.0f * deltaTime);
	}

	m_pCamera->UpdateViewMatrix();

	for (auto& iter : modelVector)
	{
		iter->Update(DirectX::XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());
		if (iter->GetRenderActive())
		{
			_renderCount++;
		}
	}

	Curr.mousePosX = Now.mousePosX;
	Curr.mousePosY = Now.mousePosY;
}

void DX11Render::Render()
{
	// �� �ȿ��ٰ� ��䷻��, ��ο������Ʈ, ���巻���� �ִ°� ȿ�����ΰ�?
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
}

void DX11Render::DrawObject()
{
	m_p3DDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	RenderAllText();

	for (auto& iter : modelVector)
	{
		iter->Render();
	}

	// �����Ͷ����� ���� ���� 
	m_p3DDeviceContext->RSSetState(0);
}

void DX11Render::EndRender()
{
	sm_SwapChain->Present(1, 0);

	// for culling check
	_renderCount = 0;

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
	/// Win32 ����
	// ���� Ŭ����

	DWORD dwStryle = WS_OVERLAPPEDWINDOW;
	RECT rect = { 0,0,_windowWidth,_windowHeight };
	AdjustWindowRect(&rect, dwStryle, false);

	// ��Ƽ����Ʈ���� �����ڵ�� �Ѿ���鼭 char* ������ ���µ�
	// �̷������� ����ȯ�� �ص� �ɱ�?
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

	// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	// ���� ����
	//hWnd = CreateWindow(
	//	// ��Ƽ����Ʈ���� �����ڵ�� �Ѿ���鼭 char* ������ ���µ�
	//	// �̷������� ����ȯ�� �ص� �ɱ�?
	//	szAppName,
	//	szAppName,
	//	WS_OVERLAPPEDWINDOW,
	//	100, 100, _windowWidth, _windowHeight,
	//	NULL, NULL, hInstance, NULL);

	hWnd = CreateWindow(
		szAppName,
		szAppName,
		dwStryle,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL);

	if (!hWnd) return S_FALSE;

	// ������ ������ ȭ�鿡 ǥ��
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
		nullptr,						// �⺻ ����͸� ����Ϸ��� nullptr�� �����Ͻʽÿ�.
		D3D_DRIVER_TYPE_HARDWARE,		// �ϵ���� �׷��� ����̹��� ����Ͽ� ��ġ�� �����մϴ�.
		nullptr,						// ����̹��� D3D_DRIVER_TYPE_SOFTWARE�� �ƴ� �� 0�̾�� �մϴ�.
		deviceFlags,					// ����� �� Direct2D ȣȯ�� �÷��׸� �����մϴ�.
		levels,							// �� ���� ������ �� �ִ� ��� ���� ����Դϴ�.
		ARRAYSIZE(levels),				// �� ����� ũ���Դϴ�.
		D3D11_SDK_VERSION,				// Windows Store ���� ��� �׻� D3D11_SDK_VERSION���� �����Ͻʽÿ�.
		&m_p3DDevice,					// ������ Direct3D ��ġ�� ��ȯ�մϴ�.
		&FeatureLevels,					// ������ ����̽��� ���� ������ ��ȯ�մϴ�.
		&m_p3DDeviceContext			    // ��ġ ��� ���ؽ�Ʈ�� ��ȯ�մϴ�.
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

	// ����� �����
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

	m_pBackBuffer->GetDesc(&m_BackBufferDesc);	// Desc ������? -> ����ۿ����� �������� ����ִ� ����ü


	// ���� ���Ľ�
	// 3D�� ���� �����Ҷ� �ʿ���
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

	//����Ʈ
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

	// ������
	//=================================================

	D3D11_RASTERIZER_DESC solidRasterDesc;	// ä��� ���
	ZeroMemory(&solidRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidRasterDesc.FillMode = D3D11_FILL_SOLID;	// ä��� ���
	solidRasterDesc.CullMode = D3D11_CULL_BACK;		// Back�� �׸��� ����
	solidRasterDesc.FrontCounterClockwise = false;	// �ݽðԸ� false = �ð�������� �׸��ڴٴ� ���̴�.
	solidRasterDesc.DepthClipEnable = true;			// �Ÿ��� ���� Ŭ������ ����

	hr = m_p3DDevice->CreateRasterizerState
	(
		&solidRasterDesc,
		m_pSolidRasterState.GetAddressOf()
	);


	//=================================================

	D3D11_RASTERIZER_DESC wireRasterDesc;	// ä��������-> �����׸�
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

	hr = CreateInput(hWnd, _windowWidth, _windowHeight);
	if (FAILED(hr))
	{
		return hr = S_FALSE;
	}

	hr = CreateUI();
	if (FAILED(hr))
	{
		return hr = S_FALSE;
	}

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

	m_pFont = Font::GetInstance(m_p3DDevice.Get(), m_p3DDeviceContext.Get());
	if (!m_pFont)
	{
		return hr = S_FALSE;
	}

	return hr;

}

HRESULT DX11Render::CreateInput(HWND hWnd, int width, int height)
{
	HRESULT hr = S_OK;
	m_pInput = new InputClass;
	if (m_pInput == nullptr)
	{
		return hr = S_FALSE;
	}
	else
	{
		m_pInput->Initialize(hInstance, hWnd, width, height);
	}
	return hr;
}

HRESULT DX11Render::CreateCamera()
{
	HRESULT hr = S_OK;

	m_pCamera = new Camera();

	// ī�޶� �����
	// ������ ���ش�.
	//	�þ߰� �� ���ν��� ũ��?
	m_pCamera->SetLens(0.25f * 3.1415926535f, 1600.0f / 1080.0f, 1.0f, 1000.0f);

	// LH(Left Hand)��������
	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 1.0f, 1000.0f);
	DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixTranspose(p));

	// ��ó���� ���� ī�޶��� ������, �Ĵٺ��� ����,UP���� ���ϱ�
	m_pCamera->LookAt(DirectX::XMFLOAT3(0.0f, 8.0f, -8.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1.0f, 0));

	return S_OK;
}

HRESULT DX11Render::CreateCube()
{
	HRESULT hr = S_OK;

	m_pNewCube = new NewCube();
	if (!m_pNewCube)
	{
		return S_FALSE;
	}
	m_pNewCube->Initialize(m_p3DDevice, m_p3DDeviceContext, m_pSolidRasterState);
	m_pNewCube->SetPosition(3.0f, 0.0f, 3.0f);
	modelVector.push_back(m_pNewCube);

	NewCube* secCube = new NewCube();
	if (!secCube)
	{
		return S_FALSE;
	}
	secCube->Initialize(m_p3DDevice, m_p3DDeviceContext, m_pSolidRasterState);
	secCube->RotateActive(true);
	modelVector.push_back(secCube);

	return S_OK;
}

HRESULT DX11Render::CreateGrid()
{
	HRESULT hr = S_OK;

	m_pGrid = new Grid();
	m_pGrid->Initialize(m_p3DDevice, m_p3DDeviceContext, m_pWireRasterState);
	if (!m_pGrid)
	{
		return hr = S_FALSE;
	}
	modelVector.push_back(m_pGrid);

	return S_OK;
}

HRESULT DX11Render::CreateAxis()
{
	HRESULT hr = S_OK;

	m_pAxis = new Axis();
	m_pAxis->Initialize(m_p3DDevice, m_p3DDeviceContext, m_pWireRasterState);
	if (!m_pAxis)
	{
		return hr = S_FALSE;
	}
	modelVector.push_back(m_pAxis);

	return S_OK;
}

HRESULT DX11Render::CreateUI()
{
	HRESULT hr = S_OK;

	m_IngameImage = new IngameImage(m_p3DDevice.Get(), m_p3DDeviceContext.Get());
	if (!m_IngameImage)
	{
		return hr = S_FALSE;
	}

	return hr;
}

LRESULT CALLBACK DX11Render::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// HDC         hdc;
	// PAINTSTRUCT ps;

	// ������ ���ν������� ��ü ������ ��������
	DX11Render* renderer = reinterpret_cast<DX11Render*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{

		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			static_mouseXpos = x;
			static_mouseYpos = y;

			if (x < 15 || y < 15 || x > 1585 || y > 1060)
			{
				//RECT clientRect;
				//GetClientRect(hWnd, &clientRect);

				//int clientCenterX = (clientRect.right - clientRect.left) / 2;
				//int clientCenterY = (clientRect.bottom - clientRect.top) / 2;

				//POINT clientCenterPoint = { clientCenterX, clientCenterY };
				//ClientToScreen(hWnd, &clientCenterPoint);

				//SetCursorPos(clientCenterPoint.x, clientCenterPoint.y);
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
	if (switchObejct == true)
	{
		Font::GetInstance()->RenderString("Camera", 1500.0f, 0.0f);
	}
	else
	{
		Font::GetInstance()->RenderString("Cube", 1500.0f, 0.0f);
	}

	//IngameImage::GetInstance()->RenderImage(L);
	//m_IngameImage->RenderImage("..\\Resources\\abcd.jpg", DirectX::XMFLOAT2(500.0f, 500.0f));

	Font::GetInstance()->RenderString("DeltaTime : ", 0.0f, 0.0f);
	Font::GetInstance()->RenderString(m_deltaTime, 110.0f, 0.0f);

	Font::GetInstance()->RenderString("FPS : ", 0.0f, 18.0f);
	Font::GetInstance()->RenderString(m_fps, 48.0f, 18.0f);

	Font::GetInstance()->RenderString("MousePosX : ", 0.0f, 36.0f);
	Font::GetInstance()->RenderString(static_mouseXpos, 120.0f, 36.0f);

	Font::GetInstance()->RenderString("MousePosY : ", 0.0f, 54.0f);
	Font::GetInstance()->RenderString(static_mouseYpos, 120.0f, 54.0f);

	Font::GetInstance()->RenderString("Last Pressed Key : ", 0.0f, 72.0f);
	Font::GetInstance()->RenderString(m_pInput->GetLastPressedkey(), 165.0f, 72.0f);

	Font::GetInstance()->RenderString("Rendering Object :", 0.0f, 90.0f);
	Font::GetInstance()->RenderString(_renderCount, 175.0f, 90.0f);
}

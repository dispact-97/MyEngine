#pragma once
#include "IDX11Render.h"

using namespace std;
/// <summary>
/// �������� õõ�� dllȭ ���Ѻ���
/// 23.06.20
/// </summary>

// DirectX 11���� 3D�׷����� �����ϰ� �׸��� ���� ��� D3D����� ���ԵǾ� �ִ�.
#pragma comment(lib, "d3d11.lib")
// ������� ����� ���� ���̺귯��
#pragma comment(lib, "dxgi.lib")
// ���̴� �����ϸ� ���
#pragma comment(lib, "d3dcompiler.lib") 

// ����Ʈ ���̺귯��
#ifdef _DEBUG
#pragma comment(lib, "..\\Lib\\Effects11d.lib")
#else
#pragma  comment(lib, "..\\Lib\\Effects11.lib")
#endif

/// ������ ��忡�� RuntimeLibrary�� ���� ����ġ�� �˻��Ǿ����ϴ�. ������ �߻���
/// -23.07.19- ���������
///  #pragma comment�� ����ؼ� ������ ���ڴٰ� �ߴٸ� �Ӽ����� ���Ӽ��� Ǯ����� �Ѵ�. ���� �ذ�
/// -23.07.21- ���������

#include "Vertex.h"

//����Ʈ �����͸� ����ϱ� ���� ���
#include <wrl\client.h>

// ����͸� ���� ���
// ������� ������ ��������� �ڵ����� ���Եȴ�
#include <d3d11_4.h>
#include <dxgi1_2.h>

class Axis;
class Cube;
class Grid;
class Camera;
class Font;

class DX11Render : public IDX11Render
{
public:
	// �̱������� �ϴ°� ������?
	DX11Render();
	virtual ~DX11Render();

	/// <summary>
	/// dll�� ����� �� �Լ����� ��� �ϳ��� ��� ����ؾ��ϳ�?
	/// __declspec(dllexport)func(int a, int b)
	/// -23.06.20 -kocca, Dispact-
	/// </summary>
	/// 
	// bool������ ���� return false�� ��ȯ�ؾ��Ѵ�.
	virtual long Initialize() override;	// �׷��� ���� �ʱ�ȭ

	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void BeginRender(float red, float green, float blue, float alpha) override;
	virtual void DrawObject() override;
	virtual void EndRender() override;

	HRESULT InitVB();

	virtual void Finalize() override;

private:
	// void OnResize(); 
	// Message�ʿ��� ����� ������ ������ ȣ���ϴ� �Լ������� �ϴ� ����
	// Render �Լ� ������ ����� �͵�

	HRESULT CreateHandleWindow();
	HRESULT CreateDevice();
	HRESULT CreateSwapChain(HWND hWnd);
	HRESULT CreateBackBuffer();
	HRESULT CreateRaster();
	// HRESULT CreateInputLayout();

	// ������Ʈ�� �����ϱ�
	 HRESULT CreateObject();	
	 HRESULT CreateCamera();
	 HRESULT CreateCube();
	 HRESULT CreateGrid();
	 HRESULT CreateAxis();

	 // ������ ����
	 HWND hWnd;
	 HINSTANCE hInstance;

	 // ������ ����,����
	 const int _windowWidth = 1800;
	 const int _windowHeight = 1080;

	 // �ð� ����
	 float m_deltaTime;

	 // �޽��� �ڵ鷯 (���� �ݹ�)
	 static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	//int mVideoCardMemory;	// ��α׿����� ������ ���ٰ� ���°ɱ�?
	//char mVideoCardDescription[128];
	// 
	// sm_SwapChain���� Preset�� �� �ʿ��ߴ� ���� ������ ����?
	//bool bVsyncEnabled = false;
	Microsoft::WRL::ComPtr<ID3D11Device> m_p3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_p3DDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;

	// ���� ���Ľ�
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	// ������ ����
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pCurrRasterState;	// ���� ������
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pSolidRasterState;	// ä��� ������
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pWireRasterState;	// ���̾� ������-> ������ ��Ÿ���ٴ¶�

	// ����ü���� ����� ���� ����
	Microsoft::WRL::ComPtr<IDXGISwapChain> sm_SwapChain;
	Microsoft::WRL::ComPtr<IDXGIDevice3> sm_dxgiDevice;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>m_pBackBuffer;

	// Direct2D�� ���� �ٸ� Factory���� ����� DXGI ��ġ ��ü�� ����ϴ�.
	Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;

	// ��Ʈ����
	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;

	// ������Ʈ
	 Axis* m_pAxis;
	 Grid* m_pGrid;
	 Cube* m_pCube;
	 Camera* m_pCamera;
	 Font* m_pFont;

	// ����
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;


};


#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Vertex.h"

// ����Ʈ ���̺귯��
//#pragma comment(lib, "..\\Lib\\Effects11d.lib")
//#include "..\\DXEffect\D3dx11effect.h"

#include <d3dcompiler.h>
#include <d3dcommon.h>

class RenderableBase
{
	// ������Ʈ�� ������ �ʿ�
public:
	virtual void ObjectSetting() abstract;

	virtual void Update() abstract;
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) abstract;
	virtual void Render() abstract;

	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

	struct MatrixBufferType					// ��� ����
	{
		DirectX::XMMATRIX _world;			// ���� ��ȯ ��� (���� -> ����)
		DirectX::XMMATRIX _view;			// �þ� ��ȯ ��� (ī�޶� ��)
		DirectX::XMMATRIX _projection;		// ���� ��ȯ ��� (���� / ����)
	};

	struct ConstantBufferData
	{
		DirectX::XMMATRIX _worldViewProjection;
	};

	// ������� �׽�Ʈ
	Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer;


	MatrixBufferType* dataptr;

	Microsoft::WRL::ComPtr<ID3D11Device>				m_3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_3DDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterState;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_VertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_IndexBuffer;		// �ε��� ����	(�������� �ε���?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;		// �������� �̰� �˾ƾ� �ұ�?->���⼭ ���ָ�ȴ�.

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11Buffer* _matrixBuffer;

	DirectX::XMMATRIX m_world;	// Transform Matrix
	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_proj;

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

	// ������ ������ ����
	DirectX::XMFLOAT3 objectPosition;

	float objectXLocation = 0.0f;
	float objectYLocation = 0.0f;
	float objectZLocation = 0.0f;


};


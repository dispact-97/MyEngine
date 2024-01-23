#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Vertex.h"

// 이펙트 라이브러리
//#pragma comment(lib, "..\\Lib\\Effects11d.lib")
//#include "..\\DXEffect\D3dx11effect.h"

#include <d3dcompiler.h>
#include <d3dcommon.h>

class RenderableBase
{
	// 업데이트와 랜더는 필요
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

	struct MatrixBufferType					// 상수 버퍼
	{
		DirectX::XMMATRIX _world;			// 월드 변환 행렬 (로컬 -> 월드)
		DirectX::XMMATRIX _view;			// 시야 변환 행렬 (카메라 뷰)
		DirectX::XMMATRIX _projection;		// 투영 변환 행렬 (원근 / 직교)
	};

	struct ConstantBufferData
	{
		DirectX::XMMATRIX _worldViewProjection;
	};

	// 상수버퍼 테스트
	Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer;


	MatrixBufferType* dataptr;

	Microsoft::WRL::ComPtr<ID3D11Device>				m_3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_3DDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterState;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_VertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_IndexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;		// 엔진에서 이걸 알아야 할까?->여기서 해주면된다.

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11Buffer* _matrixBuffer;

	DirectX::XMMATRIX m_world;	// Transform Matrix
	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_proj;

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

	// 디버깅용 포지션 변수
	DirectX::XMFLOAT3 objectPosition;

	float objectXLocation = 0.0f;
	float objectYLocation = 0.0f;
	float objectZLocation = 0.0f;


};


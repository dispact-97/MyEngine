#pragma once
//스마트 포인터를 사용하기 위한 헤더
#include <wrl\client.h>

// 이펙트 라이브러리
//#pragma comment(lib, "..\\Lib\\Effects11dx64.lib")
//#include "..\\DXEffect\D3dx11effect.h"

#include "d3dx11effect.h"

#include "Vertex.h"
#include <d3d11.h>

class TestAxis
{
public:
	TestAxis(
		Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState);
	~TestAxis();

	void InitTestAxis();

	void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);
	void Render();

private:
	Microsoft::WRL::ComPtr<ID3D11Device>				m_3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_3DDeviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_TestAxisVertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_TestAxisIndexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_TestAxisInputLayout;		// 엔진에서 이걸 알아야 할까?->여기서 해주면된다.

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_TestAxisEffect;			// 버텍스 셰이더 픽셀 셰이더를 대체할 무언가
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_TestAxisTechnique;		// 테크
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_TestAxisMatrixVariable;	// 상수버퍼를 대신할 무언가?

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterState;

	void BuildFx();
	void BuildVertexLayout();

	//이해는 아직 못했으나 반드시 사용하게 될껄?
	DirectX::XMMATRIX m_World;	// 월드 변환 행렬 (로컬 -> 월드)
	DirectX::XMMATRIX m_View;	// 시야 변환 행렬 (카메라 뷰)
	DirectX::XMMATRIX m_Proj;	// 투영 변환 행렬 (원근 / 직교)

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

};


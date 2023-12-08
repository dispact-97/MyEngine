#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Vertex.h"
#include "d3dx11effect.h"

// 이펙트 라이브러리
//#pragma comment(lib, "..\\Lib\\Effects11d.lib")
//#include "..\\DXEffect\D3dx11effect.h"

#include <d3dcompiler.h>
#include <d3dcommon.h>

#include "..\\DXTK\Effects.h"


class RenderableBase
{
	// 업데이트와 랜더는 필요
protected:
	virtual void ObjectSetting() abstract;

	virtual void Update() abstract;
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) abstract;
	virtual void Render() abstract;

	Microsoft::WRL::ComPtr<ID3D11Device>				m_3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_3DDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterState;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_VertexBuffer;		// 정점 버퍼		(정점들의 버퍼?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_IndexBuffer;		// 인덱스 버퍼	(정점들의 인덱스?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;		// 엔진에서 이걸 알아야 할까?->여기서 해주면된다.

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_Effect;			// 버텍스 셰이더 픽셀 셰이더를 대체할 무언가
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_Technique;		// 테크
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable>	m_MatrixVariable;	// 상수버퍼를 대신할 무언가?

	DirectX::XMMATRIX m_World;	// 월드 변환 행렬 (로컬 -> 월드)
	DirectX::XMMATRIX m_View;	// 시야 변환 행렬 (카메라 뷰)
	DirectX::XMMATRIX m_Proj;	// 투영 변환 행렬 (원근 / 직교)

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

};
#pragma once
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <wrl/client.h>
#include <d3d11.h>

#include "Vertex.h"

class ModelInterface
{
public:
	virtual HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState) abstract;
	virtual void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) abstract;
	virtual void Render() abstract;
	virtual void Finalzie() abstract;

	virtual DirectX::XMFLOAT3 GetPosition() abstract;

	DirectX::XMFLOAT3 _objectScreenLocation;
};
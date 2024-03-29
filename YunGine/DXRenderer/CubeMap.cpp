#include "CubeMap.h"

CubeMap::CubeMap()
	:_world(),
	_proj(),
	_view()
{

}

CubeMap::~CubeMap()
{

}

HRESULT CubeMap::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState)
{
	HRESULT hr = S_OK;

	hr = SetDevice(device, deviceContext, rasterState);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetVertexBuffer();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetIndexBuffer();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetConstantBuffer();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetShader();
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void CubeMap::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{

}

void CubeMap::Render()
{

}

void CubeMap::Finalzie()
{

}

DirectX::XMFLOAT3 CubeMap::GetPosition()
{
	return DirectX::XMFLOAT3(0,0,0);
}

bool CubeMap::SetRenderActive(bool isActive)
{
	return _renderActive = isActive;
}

bool CubeMap::GetRenderActive()
{
	return _renderActive;
}

HRESULT CubeMap::SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState)
{
	HRESULT hr = S_OK;

	_pDevice = device;
	_pDeviceContext = deviceContext;
	_pRasterState = rasterState;

	if (!_pDevice || !_pDeviceContext || !_pRasterState)
	{
		return E_POINTER;
	}

	return hr;
}

HRESULT CubeMap::SetVertexBuffer()
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CubeMap::SetIndexBuffer()
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CubeMap::SetConstantBuffer()
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CubeMap::SetTexture()
{
	HRESULT hr = S_OK;


	return hr;
}

HRESULT CubeMap::SetShader()
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CubeMap::CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;

	return hr;
}

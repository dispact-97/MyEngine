#include "TextureClass.h"
#include "TextureLoader.h"

TextureClass* TextureClass::_pInstance = nullptr;

TextureClass::TextureClass()
	:_TextureResource()
{

}

TextureClass::~TextureClass()
{

}

HRESULT TextureClass::SetTexture(std::string filePath,ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr = S_OK;

	Microsoft::WRL::ComPtr<ID3D11Resource> texResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texSRV = nullptr;
	hr = DirectX::CreateDDSTextureFromFile(device, filePath.c_str(), texResource.Get(), texSRV.Get());
	if (FAILED(hr))
	{
		return hr;
	}

	deviceContext->PSSetShaderResources(0, 1, texSRV.GetAddressOf());

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 샘플러의 필터링 모드 설정
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // 텍스처의 가로 방향 주소 모드 설정
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // 텍스처의 세로 방향 주소 모드 설정
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // 텍스처의 깊이 방향 주소 모드 설정
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // 비교 함수 설정
	samplerDesc.MinLOD = 0; // 텍스처의 최소 LOD 설정
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // 텍스처의 최대 LOD 설정

	// 샘플러 상태 객체 생성
	hr = device->CreateSamplerState(&samplerDesc, &_samplerState);
	if (FAILED(hr))
	{
		return hr;
	}

	deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

	ID3DBlob* vertexShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"..\\Shaders\\TextureVShader.hlsl", "TextureVertexShader", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		nullptr,
		&_vertexShader
	);
	if (FAILED(hr))
	{
		return hr;
	}

	ID3DBlob* PixelShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"..\\Shaders\\TexturePShader.hlsl", "TexturePixelShader", "ps_5_0", &PixelShaderBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = _pDevice->CreatePixelShader(
		PixelShaderBuffer->GetBufferPointer(),
		PixelShaderBuffer->GetBufferSize(),
		nullptr,
		&_pixelShader
	);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = _pDevice->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&_inputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	//메모리 해제
	if (PixelShaderBuffer)
	{
		PixelShaderBuffer->Release();
	}
	if (vertexShaderBuffer)
	{
		vertexShaderBuffer->Release();
	}


	return hr;
}


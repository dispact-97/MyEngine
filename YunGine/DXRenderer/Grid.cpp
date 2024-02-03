#include <vector>

#include "Grid.h"


Grid::Grid()
	: _objectPosition()
{

}

Grid::~Grid()
{

}

HRESULT Grid::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState)
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

void Grid::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	DirectX::XMMATRIX traslation = DirectX::XMMatrixTranslation(_objectPosition.x, _objectPosition.y, _objectPosition.z);
	_world = traslation * world;
	_view = view;
	_proj = projection;
}

void Grid::Render()
{
	_pDeviceContext->IASetInputLayout(_inputLayout.Get());
	_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);		// ������ �����Ҷ� �̺κ��� LINELIST��

	_pDeviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	_pDeviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	_world = DirectX::XMMatrixTranspose(_world);
	_view = DirectX::XMMatrixTranspose(_view);
	_proj = DirectX::XMMatrixTranspose(_proj);

	_pDeviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->_world = _world;
	dataPtr->_view = _view;
	dataPtr->_projection = _proj;

	_pDeviceContext->Unmap(_constantBuffer.Get(), 0);

	if (_vertexBuffer && _indexBuffer && _constantBuffer)
	{
		_pDeviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		_pDeviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_pDeviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	_pDeviceContext->RSSetState(_pRasterState.Get());

	_pDeviceContext->DrawIndexed(indexCount, 0, 0);

	// �׸���� ������ؽ�Ʈ ���ʿ�
}

void Grid::Finalzie()
{

}

HRESULT Grid::SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState)
{
	HRESULT result = S_OK;

	_pDevice = device;
	_pDeviceContext = deviceContext;
	_pRasterState = rasterState;

	if (!_pDevice || !_pDeviceContext || !_pRasterState)
	{
		return result;
	}

	return result;
}

HRESULT Grid::SetVertexBuffer()
{
	HRESULT result = S_OK;

ColorVertex GridVertex[] =
{
    // ���ؽ� �迭
    {DirectX::XMFLOAT3(-5.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 0
    {DirectX::XMFLOAT3(-4.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 1
    {DirectX::XMFLOAT3(-3.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 2
    {DirectX::XMFLOAT3(-2.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 3
    {DirectX::XMFLOAT3(-1.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 4
    {DirectX::XMFLOAT3(0.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 5
    {DirectX::XMFLOAT3(1.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 6
    {DirectX::XMFLOAT3(2.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 7
    {DirectX::XMFLOAT3(3.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 8
    {DirectX::XMFLOAT3(4.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 9
    {DirectX::XMFLOAT3(5.f,0.f,-5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 10

	// ���ؽ� �迭
	{DirectX::XMFLOAT3(-5.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 11
	{DirectX::XMFLOAT3(-4.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 12
	{DirectX::XMFLOAT3(-3.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 13
	{DirectX::XMFLOAT3(-2.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 14
	{DirectX::XMFLOAT3(-1.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 15
	{DirectX::XMFLOAT3(0.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 16
	{DirectX::XMFLOAT3(1.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 17
	{DirectX::XMFLOAT3(2.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 18
	{DirectX::XMFLOAT3(3.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 19
	{DirectX::XMFLOAT3(4.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 20
	{DirectX::XMFLOAT3(5.f,0.f,5.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 21

	// ���ؽ� �迭
	{DirectX::XMFLOAT3(-5.f,0.f,-4.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 22
	{DirectX::XMFLOAT3(-5.f,0.f,-3.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 23
	{DirectX::XMFLOAT3(-5.f,0.f,-2.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 24
	{DirectX::XMFLOAT3(-5.f,0.f,-1.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 25
	{DirectX::XMFLOAT3(-5.f,0.f,0.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 26
	{DirectX::XMFLOAT3(-5.f,0.f,1.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 27
	{DirectX::XMFLOAT3(-5.f,0.f,2.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 28
	{DirectX::XMFLOAT3(-5.f,0.f,3.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 29
	{DirectX::XMFLOAT3(-5.f,0.f,4.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 30

	// ���ؽ� �迭
	{DirectX::XMFLOAT3(5.f,0.f,-4.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 31
	{DirectX::XMFLOAT3(5.f,0.f,-3.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 32
	{DirectX::XMFLOAT3(5.f,0.f,-2.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 33
	{DirectX::XMFLOAT3(5.f,0.f,-1.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 34
	{DirectX::XMFLOAT3(5.f,0.f,-0.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 35
	{DirectX::XMFLOAT3(5.f,0.f,1.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 36
	{DirectX::XMFLOAT3(5.f,0.f,2.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 37
	{DirectX::XMFLOAT3(5.f,0.f,3.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 38
	{DirectX::XMFLOAT3(5.f,0.f,4.f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 39
};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = ARRAYSIZE(GridVertex) * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = GridVertex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	result = _pDevice->CreateBuffer(
		&bufferDesc,
		&InitData,
		&_vertexBuffer
	);

	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Grid::SetIndexBuffer()
{
	HRESULT result = S_OK;

	UINT indices[] =
	{
		// x -5���� 5���� ���μ�
		0,11,
		1,12,
		2,13,
		3,14,
		4,15,
		5,16,
		6,17,
		7,18,
		8,19,
		9,20,
		10,21,

		0,10,
		11,21,

		22,31,
		23,32,
		24,33,
		25,34,
		26,35,
		27,36,
		28,37,
		29,38,
		30,39
	};

	indexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = indexCount * sizeof(UINT);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInit;
	indexInit.pSysMem = indices;		//���� �ּҸ� �ִ´�
	indexInit.SysMemPitch = 0;
	indexInit.SysMemSlicePitch = 0;

	result = _pDevice->CreateBuffer(
		&indexBufferDesc,
		&indexInit,
		&_indexBuffer
	);

	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Grid::SetConstantBuffer()
{
	HRESULT result = S_OK;

	D3D11_BUFFER_DESC _constantBufferDesc;
	_constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_constantBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	_constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_constantBufferDesc.MiscFlags = 0;
	_constantBufferDesc.StructureByteStride = 0;

	result = _pDevice->CreateBuffer(&_constantBufferDesc, nullptr, &_constantBuffer);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT Grid::SetShader()
{
	HRESULT result = S_OK;

	ID3DBlob* vertexShaderBuffer = nullptr;
	result = CompileShaderFromFile(L"..\\Shaders\\VertexShader.hlsl", "main", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(result))
	{
		return result;
	}

	result = _pDevice->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		nullptr,
		&_vertexShader
	);
	if (FAILED(result))
	{
		return result;
	}

	ID3DBlob* PixelShaderBuffer = nullptr;
	result = CompileShaderFromFile(L"..\\Shaders\\PixelShader.hlsl", "main", "ps_5_0", &PixelShaderBuffer);
	if (FAILED(result))
	{
		return result;
	}

	result = _pDevice->CreatePixelShader(
		PixelShaderBuffer->GetBufferPointer(),
		PixelShaderBuffer->GetBufferSize(),
		nullptr,
		&_pixelShader
	);

	if (FAILED(result))
	{
		return result;
	}


	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	result = _pDevice->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&_inputLayout);

	if (FAILED(result))
	{
		return result;
	}

	//�޸� ����
	PixelShaderBuffer->Release();
	vertexShaderBuffer->Release();

	return result;
}

HRESULT Grid::CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob = nullptr;

	hr = D3DCompileFromFile(
		filename,							 // ���̴� ���� ���
		nullptr,                             // ��ũ�� ���� (�Ϲ������� nullptr ���)
		nullptr,                             // include �������̽� (�Ϲ������� nullptr ���)
		entryPoint,                          // ���̴� ������ �Լ� �̸�
		shaderModel,                         // ���̴� ��������
		shaderFlags,						 // ������ �ɼ� (����� ���� ����)
		0,                                   // �߰����� �÷��� (�Ϲ������� 0 ���)
		blobOut,							 // �����ϵ� ����� ������ ID3DBlob ������
		&errorBlob                           // ������ ���� ������ ������ ID3DBlob ������
	);

	// ������ cso������ ���� �����ϰ� ������ ������ blobOut�κ��� ���� �����ϴ� �κ��� ������ �Ѵ�.

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			// ���� ��� Ȥ�� ���
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		return hr;
	}

	if (errorBlob)
	{
		errorBlob->Release();
	}

	return hr;
}

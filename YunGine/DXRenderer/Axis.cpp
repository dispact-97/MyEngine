#include "Axis.h"

Axis::Axis()
	: _objectPosition()
{

}

Axis::~Axis()
{

}

HRESULT Axis::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState)
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

void Axis::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	DirectX::XMMATRIX traslation = DirectX::XMMatrixTranslation(_objectPosition.x, _objectPosition.y, _objectPosition.z);
	_world = traslation * world;
	_view = view;
	_proj = projection;
}

void Axis::Render()
{
	_pDeviceContext->IASetInputLayout(_inputLayout.Get());
	_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

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

}

void Axis::Finalzie()
{

}

DirectX::XMFLOAT3 Axis::GetPosition()
{
	return _objectPosition;
}

bool Axis::GetRenderActive()
{
	return _renderActive;
}

HRESULT Axis::SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState)
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

HRESULT Axis::SetVertexBuffer()
{
	HRESULT result = S_OK;

	Vertex axisVertex[] =
	{
		{DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},	//	x축	
		{DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},	//	x축	

		{DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},	//	y축	
		{DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},	//	y축	

		{DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},	//	z축	
		{DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},	//	z축	
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = ARRAYSIZE(axisVertex) * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = axisVertex;
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

HRESULT Axis::SetIndexBuffer()
{
	HRESULT result = S_OK;

	UINT indices[] =
	{
		0,1,

		2,3,

		4,5
	};

	UINT boxindexcount = ARRAYSIZE(indices);
	indexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = boxindexcount * sizeof(UINT);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInit;
	indexInit.pSysMem = indices;
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

HRESULT Axis::SetConstantBuffer()
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

HRESULT Axis::SetShader()
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

	//메모리 해제
	PixelShaderBuffer->Release();
	vertexShaderBuffer->Release();

	return result;
}

HRESULT Axis::CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob = nullptr;

	hr = D3DCompileFromFile(
		filename,							 // 쉐이더 파일 경로
		nullptr,                             // 매크로 정의 (일반적으로 nullptr 사용)
		nullptr,                             // include 인터페이스 (일반적으로 nullptr 사용)
		entryPoint,                          // 쉐이더 진입점 함수 이름
		shaderModel,                         // 쉐이더 프로파일
		shaderFlags,						 // 컴파일 옵션 (디버그 정보 포함)
		0,                                   // 추가적인 플래그 (일반적으로 0 사용)
		blobOut,							 // 컴파일된 결과를 저장할 ID3DBlob 포인터
		&errorBlob                           // 컴파일 오류 정보를 저장할 ID3DBlob 포인터
	);

	// 지금은 cso파일을 따로 저장하고 있지는 않지만 blobOut부분을 따로 저장하는 부분을 만들어야 한다.

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			// 에러 출력 혹은 기록
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

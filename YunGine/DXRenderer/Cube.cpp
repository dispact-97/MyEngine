#include <fstream>
#include <vector>
#include "Cube.h"
#include "TextureLoader.h"

#include "Font.h"

Cube::Cube(
	Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState)
{
	pDevice.CopyTo(m_3DDevice.GetAddressOf());
	pDeviceContext.CopyTo(m_3DDeviceContext.GetAddressOf());
	pRasterState.CopyTo(m_RasterState.GetAddressOf());

	ObjectSetting();
}

Cube::~Cube()
{

}

void Cube::Update()
{

}

void Cube::Move(float x, float y, float z)
{
	objectPosition.x += x;
	objectPosition.y += y;
	objectPosition.z += z;
}

void Cube::LocationTo2D()
{
	//DirectX::XMVECTOR worldPosition = DirectX::XMVectorSet(objectPosition.x, objectPosition.y, objectPosition.z, 1.0f);
	////DirectX::XMVECTOR screenPosition = XMVector3Project(worldPosition, 0, 0, _windowWidth, _windowHeight, 0.0f, 1.0f, m_proj, m_view, m_world);
	//DirectX::XMVECTOR screenPosition = XMVector3Project(worldPosition, 0, 0, 1600, 1080, 0.0f, 1.0f, m_proj, m_view, m_world);

	//// screenPosition에서 스크린 좌표를 추출하여 저장
	//// 텍스트를 그릴위치
	//objectXLocation = DirectX::XMVectorGetX(screenPosition);
	//objectYLocation = DirectX::XMVectorGetY(screenPosition);
	//objectZLocation = DirectX::XMVectorGetZ(screenPosition);

	// Cube의 월드 행렬을 사용하여 화면 좌표로 변환
	DirectX::XMFLOAT4 screenPosition;
	DirectX::XMVECTOR worldPosition = DirectX::XMLoadFloat3(&objectPosition);
	DirectX::XMVECTOR projectedPosition =
		DirectX::XMVector3Project(
			worldPosition,
			0,								// 스크린 왼쪽 모서리 x
			0,								// 스크린 왼쪽 모서리 y
			1600,							// 스크린 영역 너비	_windowWidth
			1080,							// 스크린 영역 높이	_windowHeight
			0.0f,							// 깊이 버퍼 최소값
			1.0f,							// 깊이 버퍼 최대값
			m_proj, m_view, m_world);

	DirectX::XMStoreFloat4(&screenPosition, projectedPosition);

	// 변환된 좌표를 멤버 변수에 할당
	objectXLocation = screenPosition.x;
	objectYLocation = screenPosition.y;
	objectZLocation = screenPosition.z;
}

void Cube::ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	DirectX::XMMATRIX traslation = DirectX::XMMatrixTranslation(objectPosition.x, objectPosition.y, objectPosition.z);
	m_world = traslation * world;
	m_view = view;
	m_proj = projection;

	LocationTo2D();
}

void Cube::Render()
{
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(TexVertex);
	UINT offset = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	m_world = DirectX::XMMatrixTranspose(m_world);
	m_view = DirectX::XMMatrixTranspose(m_view);
	m_proj = DirectX::XMMatrixTranspose(m_proj);

	m_3DDeviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//memcpy(mappedResource.pData, &m_cbdata, sizeof(MatrixBufferType));

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->_world = m_world;
	dataPtr->_view = m_view;
	dataPtr->_projection = m_proj;

	m_3DDeviceContext->Unmap(m_constantBuffer.Get(), 0);

	//m_3DDeviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &m_cbdata, 0, 0);

	m_3DDeviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

	if (m_VertexBuffer && m_IndexBuffer)
	{
		m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);

	Font::GetInstance()->ObjectDebugText(this);
	// 마지막에 넣으면 큐브도 나오고 텍스트도 나온다.
}

void Cube::ObjectSetting()
{
	// 위치 초기화
	objectPosition = { 2.0f,0.0f,2.0f };
	HRESULT hr = S_OK;

	// 정중앙
	TexVertex boxVertex[] =
	{
		// 앞면
		{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)}, // 0
		{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)},  // 1
		{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)},   // 2
		{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},  // 3

		// 뒷면
		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},  // 4
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)},   // 5
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)},    // 6
		{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)},   // 7

		// top
		{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)},  // 8
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)},   // 9
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)},    // 10
		{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},   // 11

		// bottom
		{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)},  // 12
		{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},   // 13
		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)},  // 14
		{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)}, // 15

		// left
		{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)},   // 16
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)},    // 17
		{DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)},   // 18
		{DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},  // 19

		// right
		{DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f)},  // 20
		{DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f)},   // 21
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f)},    // 22
		{DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f)},   // 23
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = 24 * sizeof(TexVertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���۰� ���������ο� ���ε��Ǵ� ����ĺ�
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = boxVertex;	// const ��ȿ-> �ʱ�ȭ �����Ϳ� ���� �������̴�.
	InitData.SysMemPitch = 0;		// �ؽ�ó �� �� ���� �κп��� ������ ������ �Ÿ�(byte)
	InitData.SysMemSlicePitch = 0;	// �� ���� ������ ���ۺ��� ���� ���ر����� �Ÿ�(byte)


	hr = m_3DDevice->CreateBuffer
	(
		&bufferDesc,
		&InitData,
		&m_VertexBuffer
	);

	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		6, 5, 4,
		6, 4, 7,

		// top
		8, 9, 10,
		8, 10 ,11,

		// bottom
		15, 12, 13,
		15, 13, 14,

		// left
		16, 17, 18,
		16, 18, 19,

		// right
		21, 22, 23,
		20, 21, 23
	};

	UINT boxindexcount = ARRAYSIZE(indices);
	indexcount = ARRAYSIZE(indices);

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

	hr = m_3DDevice->CreateBuffer(
		&indexBufferDesc,
		&indexInit,
		&m_IndexBuffer
	);

	// 테스트
	D3D11_BUFFER_DESC _constantBufferDesc;
	_constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_constantBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	_constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_constantBufferDesc.MiscFlags = 0;
	_constantBufferDesc.StructureByteStride = 0;

	hr = m_3DDevice->CreateBuffer(&_constantBufferDesc, nullptr, m_constantBuffer.GetAddressOf());

	CreateShader();
	GetTextureFile();

}

HRESULT Cube::CreateShader()
{
	HRESULT hr = S_OK;
	ID3DBlob* vertexShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"..\\Shaders\\VertexShader.hlsl", "main", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_3DDevice->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		nullptr,
		&_vertexShader);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = m_3DDevice->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_InputLayout);

	if (FAILED(hr))
	{
		// 오류 처리 및 버퍼 해제
		if (vertexShaderBuffer)
		{
			vertexShaderBuffer->Release();
		}

		if (vertexShaderBuffer)
		{
			vertexShaderBuffer->Release();
		}
	}
}

HRESULT Cube::CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(filename, nullptr, nullptr, entryPoint, shaderModel,
		shaderFlags, 0, blobOut, &errorBlob);

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

void Cube::BuildVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

void Cube::GetTextureFile()
{
	HRESULT hr = S_OK;

	Microsoft::WRL::ComPtr<ID3D11Resource> texResource = nullptr;	// Release�� ����� �� �� ������?
	DirectX::CreateDDSTextureFromFile(m_3DDevice.Get(), L"../Textures/WoodCrate01.dds", &texResource, &m_DiffuseMapSRV);

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}
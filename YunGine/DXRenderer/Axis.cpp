#include "Axis.h"
#include <fstream>
#include <sstream>
#include <vector>

Axis::Axis(
	Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, 
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState)	// wireRaster�� ����
{
	pDevice.CopyTo(m_3DDevice.GetAddressOf());
	pDeviceContext.CopyTo(m_3DDeviceContext.GetAddressOf());
	pRasterState.CopyTo(m_RasterState.GetAddressOf());

	ObjectSetting();
}

Axis::~Axis()
{

}

void Axis::Update()
{

}

void Axis::ObjectSetting()  
{
	HRESULT hr = S_OK;

	ColorVertex axisvertex[] =
	{
		{DirectX::XMFLOAT3(0.f,0.01f,0.f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)},
		{DirectX::XMFLOAT3(15.f,0.01f,0.f),DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f)},

		{DirectX::XMFLOAT3(0.f,0.f,0.f),DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},
		{DirectX::XMFLOAT3(0.f,15.f,0.f),DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},

		{DirectX::XMFLOAT3(0.f,0.f,0.f),DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(0.f,0.f,15.f),DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f)}
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = 6 * sizeof(ColorVertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���۰� ���������ο� ���ε��Ǵ� ����ĺ�
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// ���� �ʱ�ȭ�� ��������->D3D����̽��� �������� ����ֱ� ������ ������ �Ѱ��ش�.
	// �ؽ�ó �Ҷ� ����Ѵ�.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = axisvertex;	// const ��ȿ-> �ʱ�ȭ �����Ϳ� ���� �������̴�.
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
		0,1,	// x

		2,3,	// y

		4,5		// z
	};

	UINT axiscount = ARRAYSIZE(indices);
	indexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = axiscount * sizeof(UINT);
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
		m_IndexBuffer.GetAddressOf()
	);

	// �׽�Ʈ
	D3D11_BUFFER_DESC _constantBufferDesc;
	_constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_constantBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	_constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_constantBufferDesc.MiscFlags = 0;
	_constantBufferDesc.StructureByteStride = 0;

	hr = m_3DDevice->CreateBuffer(&_constantBufferDesc, nullptr, m_constantBuffer.GetAddressOf());

	CreateShader();
}

void Axis::ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	m_world = world;
	m_view = view;
	m_proj = projection;
}


void Axis::Render()
{
	HRESULT hr;

	// �Է� ��ġ ��ü ����
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �ε������ۿ� ���ؽ� ���� ����
	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;
	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_axisVertexBuffer�� AddressOf���̰� ���ϱ�-> &�� �ʱ�ȭ�� �ع�����.
	m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	m_world = DirectX::XMMatrixTranspose(m_world);
	m_view = DirectX::XMMatrixTranspose(m_view);
	m_proj = DirectX::XMMatrixTranspose(m_proj);

	m_3DDeviceContext->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->_world = m_world;
	dataPtr->_view = m_view;
	dataPtr->_projection = m_proj;

	m_3DDeviceContext->Unmap(m_constantBuffer.Get(), 0);

	m_3DDeviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

	if (m_VertexBuffer && m_IndexBuffer)
	{
		m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	//����������Ʈ
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	m_3DDeviceContext->DrawIndexed(indexCount, 0, 0);
}

HRESULT Axis::CreateShader()
{
	HRESULT hr;
	ID3D10Blob* vertexShaderBuffer = nullptr;

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

	// Create the vertex input layout.
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
		// ���� ó�� �� ���� ����
		if (vertexShaderBuffer)
		{
			vertexShaderBuffer->Release();
		}
	}

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

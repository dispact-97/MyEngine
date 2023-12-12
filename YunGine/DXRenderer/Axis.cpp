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

	//D3D�������� ����̽��� ���⼭ �Ѱ�����ϳ�?
	hr = m_3DDevice->CreateBuffer
	(
		&bufferDesc,
		&InitData,
		m_VertexBuffer.GetAddressOf()
	);


	UINT indices[] =
	{
		0,1,	// x

		2,3,	// y

		4,5		// z
	};

	UINT axiscount = ARRAYSIZE(indices);

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


	//BuildVertexLayout();
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
	m_3DDeviceContext->VSSetShader(_vertexShader, nullptr, 0);
	m_3DDeviceContext->PSSetShader(_pixelShader, nullptr, 0);

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
	unsigned int bufferNumber;

	m_world = DirectX::XMMatrixTranspose(m_world);
	m_view = DirectX::XMMatrixTranspose(m_view);
	m_proj = DirectX::XMMatrixTranspose(m_proj);

	HRESULT hr;

	hr = m_3DDeviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->_world = m_world;
	dataPtr->_view = m_view;
	dataPtr->_projection = m_proj;

	m_3DDeviceContext->Unmap(_matrixBuffer, 0);

	bufferNumber = 0;

	m_3DDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	//����������Ʈ
	m_3DDeviceContext->RSSetState(m_RasterState.Get());


	m_3DDeviceContext->DrawIndexed(6, 0, 0);
}

void Axis::BuildVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	/// �׳� ���� �����ִ°� �����ؼ� ARRAYSIZE�� �ٲ�
	/// �ƹ� �ǹ� ���� ��
// 	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
// 		passDesc.IAInputSignatureSize, m_InputLayout.GetAddressOf()));

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

void Axis::CreateShader()
{
	HRESULT hr;

	std::ifstream vsFile("../x64/Debug/VertexShader.cso", std::ios::binary);
	std::ifstream psFile("../x64/Debug/PixelShader.cso", std::ios::binary);

	std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	m_3DDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);
	m_3DDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);


	// ���̴��� �����ΰ� ���̾ƿ��� �����ΰ� �̷��� ������ ���� �� �� �ְ��ϴ°� ������
	// �̷��� �ڵ������� �ھƵθ� ������ �� ���ٴ� ��⸦ �ϴ� �� ������?

	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = m_3DDevice->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);

	m_3DDevice->CreateInputLayout(vertexDesc, 2, vsData.data(), vsData.size(), &m_InputLayout);
}

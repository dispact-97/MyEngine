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

void Cube::ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	m_world = world;
	m_view = view;
	m_proj = projection;
}

void Cube::Render()
{
<<<<<<< Updated upstream
	Font::GetInstance()->RenderString("Stupid", 500.0f,500.0f);

	// �Է� ��ġ ��ü ����
=======
	//m_3DDeviceContext->OMSetDepthStencilState(nullptr, 0);
	Font::GetInstance()->ObjectDebugText(this);

>>>>>>> Stashed changes
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �ε������ۿ� ���ؽ� ���� ����
	UINT stride = sizeof(TexVertex);
	UINT offset = 0;

	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);
}

void Cube::ObjectSetting()
{
	HRESULT hr = S_OK;

	// 살짝 비켜나게
	//TexVertex boxVertex[] =
	//{

	//	{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 0	// �ܼ��� �ܾ���� �ȴ�-> ���� ���� ������ �÷���
	//	{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 1	// �׷��ٸ� ���� �� ���ڰ� �÷��� �ƴ϶�� ����
	//	{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 2	// �˷���� �Ѵ�.
	//	{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 3

	//				
	//	{DirectX::XMFLOAT3(1.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 4
	//	{DirectX::XMFLOAT3(1.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 5
	//	{DirectX::XMFLOAT3(2.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 6
	//	{DirectX::XMFLOAT3(2.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 7

	//	// top
	//	{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 8
	//	{DirectX::XMFLOAT3(1.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 9
	//	{DirectX::XMFLOAT3(2.f,2.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 10
	//	{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 11

	//	// bottom
	//	{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 12
	//	{DirectX::XMFLOAT3(2.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 13
	//	{DirectX::XMFLOAT3(1.f,1.f,-3.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 14
	//	{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 15

	//	// left
	//	{DirectX::XMFLOAT3(1.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 16
	//	{DirectX::XMFLOAT3(1.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 17
	//	{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 18
	//	{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 19

	//	// right
	//	{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 20
	//	{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 21
	//	{DirectX::XMFLOAT3(2.f,2.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 22
	//	{DirectX::XMFLOAT3(2.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 23
	//};

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
	//indexBufferDesc.ByteWidth = sizeof(indices); // ���̰� ����
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

	GetTextureFile();
	BuildVertexLayout();
}

void Cube::BuildVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// 	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
	// 		passDesc.IAInputSignatureSize, m_InputLayout.GetAddressOf()
	// 	));

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
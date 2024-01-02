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
	Font::GetInstance()->RenderString("Stupid", 500.0f,500.0f);

	// 입력 배치 객체 셋팅
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스 버퍼 셋팅
	UINT stride = sizeof(TexVertex);
	UINT offset = 0;

	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_cubVertexBuffer와 AddressOf차이가 뭐일까-> &는 초기화를 해버린다.
	m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	///[TW] 텍스쳐 임시 확인용 회전.
	// DirectX::XMMATRIX tw_local = DirectX::XMMatrixRotationX(remainderf(tw_rot1, 360.0f));
	// m_World = DirectX::XMMatrixMultiply(tw_local, m_World);
	// 
	// tw_local = DirectX::XMMatrixRotationY(remainderf(tw_rot2, 360.0f));
	// m_World = DirectX::XMMatrixMultiply(tw_local, m_World);


	///WVP TM등을 셋팅
	//DirectX::XMMATRIX worldViewProj = m_World * m_View * m_Proj;

	//랜더스테이트
	m_3DDeviceContext->RSSetState(m_RasterState.Get());


	m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);
}

void Cube::ObjectSetting()
{
	HRESULT hr = S_OK;

	TexVertex boxVertex[] =
	{
		// 포지션, 텍스코드
		// 앞면
		{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 0	// 단순히 긁어오면 안댐-> 들어가는 값은 여전히 컬러값
		{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 1	// 그렇다면 들어갈때 이 숫자가 컬러가 아니라는 것을
		{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 2	// 알려줘야 한다.
		{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 3

		// 뒷면							
		{DirectX::XMFLOAT3(1.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 4
		{DirectX::XMFLOAT3(1.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 5
		{DirectX::XMFLOAT3(2.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 6
		{DirectX::XMFLOAT3(2.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 7

		// top
		{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 8
		{DirectX::XMFLOAT3(1.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 9
		{DirectX::XMFLOAT3(2.f,2.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 10
		{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 11

		// bottom
		{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 12
		{DirectX::XMFLOAT3(2.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 13
		{DirectX::XMFLOAT3(1.f,1.f,-3.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 14
		{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 15

		// left
		{DirectX::XMFLOAT3(1.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 16
		{DirectX::XMFLOAT3(1.f,2.f,-3.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 17
		{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 18
		{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 19

		// right
		{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 20
		{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 21
		{DirectX::XMFLOAT3(2.f,2.f,-3.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 22
		{DirectX::XMFLOAT3(2.f,1.f,-3.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 23
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = 24 * sizeof(TexVertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 버퍼가 파이프라인에 바인딩되는 방법식별
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 버퍼 초기화는 엔진에서->D3D디바이스를 엔진에서 들고있기 때문에 정보만 넘겨준다.
	// 텍스처 할때 사용한다.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = boxVertex;	// const 무효-> 초기화 데이터에 대한 포인터이다.
	InitData.SysMemPitch = 0;		// 텍스처 한 줄 시작 부분에서 다음줄 까지의 거리(byte)
	InitData.SysMemSlicePitch = 0;	// 한 깊이 수준의 시작부터 다음 수준까지의 거리(byte)

	//D3D엔진에서 디바이스를 여기서 넘겨줘야하나?
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
	//indexBufferDesc.ByteWidth = sizeof(indices); // 차이가 없다
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

	Microsoft::WRL::ComPtr<ID3D11Resource> texResource = nullptr;	// Release를 제대로 할 수 있을까?
	DirectX::CreateDDSTextureFromFile(m_3DDevice.Get(), L"../Textures/WoodCrate01.dds", &texResource, &m_DiffuseMapSRV);
	// effect 라이브러리 를 추가해야함
	// 라이브러리 추가후 내용을 읽어 오지 못해서 엔진있는 곳에 TextureLoader를 가져왔음

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

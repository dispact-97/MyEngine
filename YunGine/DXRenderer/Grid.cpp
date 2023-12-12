#include "Grid.h"

Grid::Grid(
	Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState)
{
	pDevice.CopyTo(m_3DDevice.GetAddressOf());
	pDeviceContext.CopyTo(m_3DDeviceContext.GetAddressOf());
	pRasterState.CopyTo(m_RasterState.GetAddressOf());

	ObjectSetting();
}

Grid::~Grid()
{

}

void Grid::ObjectSetting()
{
	HRESULT hr = S_OK;

	ColorVertex GridVertex[] =
	{
		{DirectX::XMFLOAT3(-5.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 0
		{DirectX::XMFLOAT3(-4.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 1
		{DirectX::XMFLOAT3(-3.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 2
		{DirectX::XMFLOAT3(-2.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 3
		{DirectX::XMFLOAT3(-1.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 4
		{DirectX::XMFLOAT3(0.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 5
		{DirectX::XMFLOAT3(1.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 6
		{DirectX::XMFLOAT3(2.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 7
		{DirectX::XMFLOAT3(3.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 8
		{DirectX::XMFLOAT3(4.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 9
		{DirectX::XMFLOAT3(5.f,0.f,5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 10

		// 가로점
		{DirectX::XMFLOAT3(-5.f,0.f,4.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 11
		{DirectX::XMFLOAT3(5.f,0.f,4.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 12

		{DirectX::XMFLOAT3(-5.f,0.f,3.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,3.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,2.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,2.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,1.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,1.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,0.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,0.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,-1.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,-1.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,-2.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,-2.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,-3.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,-3.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,-4.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,-4.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},

		{DirectX::XMFLOAT3(-5.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},
		{DirectX::XMFLOAT3(5.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 30

		//세로 점
		{DirectX::XMFLOAT3(-4.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 31
		{DirectX::XMFLOAT3(-3.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 32
		{DirectX::XMFLOAT3(-2.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 33
		{DirectX::XMFLOAT3(-1.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 34
		{DirectX::XMFLOAT3(0.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 35
		{DirectX::XMFLOAT3(1.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 36
		{DirectX::XMFLOAT3(2.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 37
		{DirectX::XMFLOAT3(3.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 38
		{DirectX::XMFLOAT3(4.f,0.f,-5.f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)},	// 39

	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(GridVertex);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 버퍼가 파이프라인에 바인딩되는 방법식별
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 버퍼 초기화는 엔진에서->D3D디바이스를 엔진에서 들고있기 때문에 정보만 넘겨준다.
	// 텍스처 할때 사용한다.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = GridVertex;	// const 무효-> 초기화 데이터에 대한 포인터이다.
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
		// x -5에서 5까지 가로선
		0,10,
		11,12,
		13,14,
		15,16,
		17,18,
		19,20,
		21,22,
		23,24,
		25,26,
		27,28,
		29,30,

		// z5부터 -5까지 세로선
		0,29,
		1,31,
		2,32,
		3,33,
		4,34,
		5,35,
		6,36,
		7,37,
		8,38,
		9,39,
		10,30

	};


	UINT Gridindexcount = ARRAYSIZE(indices);
	indexcount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = Gridindexcount * sizeof(UINT);	// 차이가 없다
	//indexBufferDesc.ByteWidth = sizeof(indices); 
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

	BuildVertexLayout();
}

void Grid::BuildVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

// 	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
// 		passDesc.IAInputSignatureSize, m_InputLayout.GetAddressOf()
// 	));

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

void Grid::Update()
{

}

void Grid::ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	m_world = world;
	m_view = view;
	m_proj = projection;
}

void Grid::Render()
{
	// 입력 배치 객체 셋팅
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스 버퍼 셋팅
	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;
	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_cubVertexBuffer와 AddressOf차이가 뭐일까-> &는 초기화를 해버린다.
	m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//랜더스테이트
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);
}

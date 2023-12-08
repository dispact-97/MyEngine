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

	BuildFX();
	BuildVertexLayout();
}

void Grid::BuildFX()
{
	HRESULT hr = S_OK;
	//이펙트를 쓰는부분은 차차 해보자

	/// 컴파일된 파일도 괜찮고
	/// 컴파일 하는 것도 문제없이 돌아감.

	UINT shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;	// 쉐이더 컴파일시 엄격한 문법 검사를 수행하도록 하는 컴파일 플래그

#if defined( DEBUG ) || defined( _DEBUG )	// 디버그 모드에서 쉐이더 컴파일시 디버깅에 필요한 정보를 추가
	shaderFlag |= D3D10_SHADER_DEBUG;		// 최적화 과정을 건너뛰도록 설정하는 역할이다.
	shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* compiledShader;
	ID3DBlob* compilationMsgs;


	LPCWSTR shaderFile = L"../fx/color.fx";
	LPCSTR shaderEntryPoint = "main";
	LPCSTR shaderTarget = "fx_5_0";

	hr = D3DCompileFromFile(shaderFile, nullptr, nullptr, shaderEntryPoint, shaderTarget, shaderFlag, 0, &compiledShader, &compilationMsgs);

	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_3DDevice.Get(), m_Effect.GetAddressOf());

	m_Technique = m_Effect->GetTechniqueByName("ColorTech");
	m_MatrixVariable = m_Effect->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Grid::BuildVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3DX11_PASS_DESC passDesc;
	m_Technique->GetPassByIndex(0)->GetDesc(&passDesc);

	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, m_InputLayout.GetAddressOf()
	));

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
	m_World = world;
	m_View = view;
	m_Proj = projection;
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

	///WVP TM등을 셋팅
	DirectX::XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_MatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	//랜더스테이트
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	//테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	m_Technique->GetDesc(&techDesc);

	//랜더패스
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_Technique->GetPassByIndex(p)->Apply(0, m_3DDeviceContext.Get());

		m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);
	}
}

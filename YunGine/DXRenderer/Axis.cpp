#include "Axis.h"
#include <fstream>
#include <sstream>
#include <vector>

Axis::Axis(
	Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, 
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState)	// wireRaster로 받음
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
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 버퍼가 파이프라인에 바인딩되는 방법식별
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// 버퍼 초기화는 엔진에서->D3D디바이스를 엔진에서 들고있기 때문에 정보만 넘겨준다.
	// 텍스처 할때 사용한다.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = axisvertex;	// const 무효-> 초기화 데이터에 대한 포인터이다.
	InitData.SysMemPitch = 0;		// 텍스처 한 줄 시작 부분에서 다음줄 까지의 거리(byte)
	InitData.SysMemSlicePitch = 0;	// 한 깊이 수준의 시작부터 다음 수준까지의 거리(byte)

	//D3D엔진에서 디바이스를 여기서 넘겨줘야하나?
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

	// 입력 배치 객체 셋팅
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스 버퍼 셋팅
	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;
	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_axisVertexBuffer와 AddressOf차이가 뭐일까-> &는 초기화를 해버린다.
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

	//랜더스테이트
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

	/// 그냥 숫자 적혀있는게 불편해서 ARRAYSIZE로 바꿈
	/// 아무 의미 없긴 함
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


	// 셰이더도 만들어두고 레이아웃도 만들어두고 이런거 저런거 갖다 쓸 수 있게하는게 좋겠지
	// 이렇게 코드적으로 박아두면 안좋을 것 같다는 얘기를 하는 것 같은데?

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

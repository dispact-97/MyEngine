#include "TestAxis.h"
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#include <d3dcommon.h>

/// 포인터로 받아오던 디바이스, 컨텍스트를 comptr로 받아도록 변경
TestAxis::TestAxis(
	Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState)
{
	pDevice.CopyTo(m_3DDevice.GetAddressOf());
	pDeviceContext.CopyTo(m_3DDeviceContext.GetAddressOf());
	pRasterState.CopyTo(m_RasterState.GetAddressOf());


	InitTestAxis();
}

TestAxis::~TestAxis()
{

}

void TestAxis::InitTestAxis()
{

	//BuildGeomertryBuffers(); 함수의 내용을 여기서 해줌

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
		m_TestAxisVertexBuffer.GetAddressOf()
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
		m_TestAxisIndexBuffer.GetAddressOf()
	);

	BuildFx();
	BuildVertexLayout();

}

void TestAxis::Render()
{
	// 입력 배치 객체 셋팅
	m_3DDeviceContext->IASetInputLayout(m_TestAxisInputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스 버퍼 셋팅
	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;
	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_TestAxisVertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_axisVertexBuffer와 AddressOf차이가 뭐일까-> &는 초기화를 해버린다.
	m_3DDeviceContext->IASetIndexBuffer(m_TestAxisIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	///WVP TM등을 셋팅
	DirectX::XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_TestAxisMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	//랜더스테이트
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	//테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	m_TestAxisTechnique->GetDesc(&techDesc);

	//랜더패스
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_TestAxisTechnique->GetPassByIndex(p)->Apply(0, m_3DDeviceContext.Get());

		m_3DDeviceContext->DrawIndexed(6, 0, 0);
	}

}

void TestAxis::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	m_World = world;
	m_View = view;
	m_Proj = projection;
}

void TestAxis::BuildFx()
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

	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_3DDevice.Get(), m_TestAxisEffect.GetAddressOf());

	m_TestAxisTechnique = m_TestAxisEffect->GetTechniqueByName("ColorTech");
	m_TestAxisMatrixVariable = m_TestAxisEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void TestAxis::BuildVertexLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_TestAxisTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	/// 그냥 숫자 적혀있는게 불편해서 ARRAYSIZE로 바꿈
	/// 아무 의미 없긴 함
	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, m_TestAxisInputLayout.GetAddressOf()));

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

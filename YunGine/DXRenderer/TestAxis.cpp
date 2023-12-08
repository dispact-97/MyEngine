#include "TestAxis.h"
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#include <d3dcommon.h>

/// �����ͷ� �޾ƿ��� ����̽�, ���ؽ�Ʈ�� comptr�� �޾Ƶ��� ����
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

	//BuildGeomertryBuffers(); �Լ��� ������ ���⼭ ����

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
	// �Է� ��ġ ��ü ����
	m_3DDeviceContext->IASetInputLayout(m_TestAxisInputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �ε������ۿ� ���ؽ� ���� ����
	UINT stride = sizeof(ColorVertex);
	UINT offset = 0;
	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_TestAxisVertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_axisVertexBuffer�� AddressOf���̰� ���ϱ�-> &�� �ʱ�ȭ�� �ع�����.
	m_3DDeviceContext->IASetIndexBuffer(m_TestAxisIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	///WVP TM���� ����
	DirectX::XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_TestAxisMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	//����������Ʈ
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	//��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	m_TestAxisTechnique->GetDesc(&techDesc);

	//�����н�
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
	//����Ʈ�� ���ºκ��� ���� �غ���

	/// �����ϵ� ���ϵ� ������
	/// ������ �ϴ� �͵� �������� ���ư�.

	UINT shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;	// ���̴� �����Ͻ� ������ ���� �˻縦 �����ϵ��� �ϴ� ������ �÷���

#if defined( DEBUG ) || defined( _DEBUG )	// ����� ��忡�� ���̴� �����Ͻ� ����뿡 �ʿ��� ������ �߰�
	shaderFlag |= D3D10_SHADER_DEBUG;		// ����ȭ ������ �ǳʶٵ��� �����ϴ� �����̴�.
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

	/// �׳� ���� �����ִ°� �����ؼ� ARRAYSIZE�� �ٲ�
	/// �ƹ� �ǹ� ���� ��
	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, m_TestAxisInputLayout.GetAddressOf()));

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

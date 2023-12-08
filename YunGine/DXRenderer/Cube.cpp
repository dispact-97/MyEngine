#include <fstream>
#include <vector>
#include "Cube.h"
#include "TextureLoader.h"

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
	m_World = world;
	m_View = view;
	m_Proj = projection;
}

void Cube::Render()
{
	// �Է� ��ġ ��ü ����
	m_3DDeviceContext->IASetInputLayout(m_InputLayout.Get());
	m_3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �ε������ۿ� ���ؽ� ���� ����
	UINT stride = sizeof(TexVertex);
	UINT offset = 0;

	m_3DDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	// &m_cubVertexBuffer�� AddressOf���̰� ���ϱ�-> &�� �ʱ�ȭ�� �ع�����.
	m_3DDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	///[TW] �ؽ��� �ӽ� Ȯ�ο� ȸ��.
	// DirectX::XMMATRIX tw_local = DirectX::XMMatrixRotationX(remainderf(tw_rot1, 360.0f));
	// m_World = DirectX::XMMatrixMultiply(tw_local, m_World);
	// 
	// tw_local = DirectX::XMMatrixRotationY(remainderf(tw_rot2, 360.0f));
	// m_World = DirectX::XMMatrixMultiply(tw_local, m_World);


	///WVP TM���� ����
	DirectX::XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_textureboxMatrixVariable->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	m_GworldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_World));

	m_DiffuseMap->SetResource(m_DiffuseMapSRV.Get());

	//����������Ʈ
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	//��ũ��
	D3DX11_TECHNIQUE_DESC techDesc;
	m_textureboxTechnique->GetDesc(&techDesc);

	//�����н�
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_textureboxTechnique->GetPassByIndex(p)->Apply(0, m_3DDeviceContext.Get());

		m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);
	}
}

void Cube::ObjectSetting()
{
	HRESULT hr = S_OK;

	TexVertex boxVertex[] =
	{
		// ������, �ؽ��ڵ�
		// �ո�
		{DirectX::XMFLOAT3(1.f,1.f,-4.f),DirectX::XMFLOAT2(0.0f, 1.0f)},	// 0	// �ܼ��� �ܾ���� �ȴ�-> ���� ���� ������ �÷���
		{DirectX::XMFLOAT3(1.f,2.f,-4.f),DirectX::XMFLOAT2(0.0f, 0.0f)},	// 1	// �׷��ٸ� ���� �� ���ڰ� �÷��� �ƴ϶�� ����
		{DirectX::XMFLOAT3(2.f,2.f,-4.f),DirectX::XMFLOAT2(1.0f, 0.0f)},	// 2	// �˷���� �Ѵ�.
		{DirectX::XMFLOAT3(2.f,1.f,-4.f),DirectX::XMFLOAT2(1.0f, 1.0f)},	// 3

		// �޸�							
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
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���۰� ���������ο� ���ε��Ǵ� ����ĺ�
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// ���� �ʱ�ȭ�� ��������->D3D����̽��� �������� ����ֱ� ������ ������ �Ѱ��ش�.
	// �ؽ�ó �Ҷ� ����Ѵ�.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = boxVertex;	// const ��ȿ-> �ʱ�ȭ �����Ϳ� ���� �������̴�.
	InitData.SysMemPitch = 0;		// �ؽ�ó �� �� ���� �κп��� ������ ������ �Ÿ�(byte)
	InitData.SysMemSlicePitch = 0;	// �� ���� ������ ���ۺ��� ���� ���ر����� �Ÿ�(byte)

	//D3D�������� ����̽��� ���⼭ �Ѱ�����ϳ�?
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
	BuildFxFromCso(m_3DDevice.Get(), L"../fx/Basic.cso");
	BuildVertexLayout();
}

void Cube::BuildFX()
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

	LPCWSTR shaderFile = L"../fx/Basic.fx";
	LPCSTR shaderEntryPoint = "VS";
	LPCSTR shaderTarget = "fx_5_0";

	hr = D3DCompileFromFile(shaderFile, nullptr, nullptr, shaderEntryPoint, shaderTarget, shaderFlag, 0, &compiledShader, &compilationMsgs);

	//D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_3DDevice.Get(), m_textureboxEffect.GetAddressOf());

	m_Technique = m_textureboxEffect->GetTechniqueByName("TextureTech");
	m_textureboxMatrixVariable = m_textureboxEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
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

	D3DX11_PASS_DESC passDesc;
	m_textureboxTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	hr = (m_3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, m_InputLayout.GetAddressOf()
	));

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

void Cube::BuildFxFromCso(ID3D11Device* device, const std::wstring& filename)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HRESULT hr = D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &m_textureboxEffect);

	m_textureboxTechnique = m_textureboxEffect->GetTechniqueByName("TextureTech");
	m_textureboxMatrixVariable = m_textureboxEffect->GetVariableByName("gWorldViewProj")->AsMatrix();	// ��ķ� ������
	m_GworldMatrixVariable = m_textureboxEffect->GetVariableByName("gWorld")->AsMatrix();	// ��ķ� ������

	m_DiffuseMap = m_textureboxEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	BOOL res = m_textureboxEffect->IsValid();	// �޸𸮰� �����Ⱚ�� �ƴ����� Ȯ���ϴ� �Լ�
	res = m_textureboxTechnique->IsValid();
	res = m_textureboxMatrixVariable->IsValid();
	res = m_GworldMatrixVariable->IsValid();
	res = m_DiffuseMap->IsValid();

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
	// effect ���̺귯�� �� �߰��ؾ���
	// ���̺귯�� �߰��� ������ �о� ���� ���ؼ� �����ִ� ���� TextureLoader�� ��������

	if (FAILED(hr))
	{
		hr = S_FALSE;
	}
}

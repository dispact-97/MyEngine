#include "Model.h"
#include "Font.h"

Model::Model(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext, Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState)
{
	pDevice.CopyTo(m_3DDevice.GetAddressOf());
	pDeviceContext.CopyTo(m_3DDeviceContext.GetAddressOf());
	pRasterState.CopyTo(m_RasterState.GetAddressOf());

	ObjectSetting();
}

Model::~Model()
{

}

void Model::Update()
{

}

void Model::Render()
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

	//����������Ʈ
	m_3DDeviceContext->RSSetState(m_RasterState.Get());

	//m_3DDeviceContext->DrawIndexed(indexcount, 0, 0);
}

void Model::ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	m_world = world;
	m_view = view;
	m_proj = projection;
}

void Model::LoadData(const std::string& filePath)
{
	FbxLoader* SampleLoader = new FbxLoader(filePath);

	SampleLoader->LoadModel(filePath,this);



}

bool Model::ObjectSettingSample()
{
	if (m_3DDevice == nullptr || m_3DDeviceContext == nullptr || m_RasterState == nullptr)
	{
		return false;
	}

	return true;
}

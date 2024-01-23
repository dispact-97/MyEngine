#pragma once
#include <iostream>
#include "RenderableBase.h"

class Cube final : public RenderableBase
{
public:
	Cube(
		Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState);
	~Cube();

	virtual void Update() override;
	// �׽���
	void Move(float x, float y, float z);
	void LocationTo2D();
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) override;
	virtual void Render() override;

private:
	virtual void ObjectSetting() override;

	HRESULT CreateShader();
	HRESULT CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);
	void BuildVertexLayout();
	void GetTextureFile();

	int indexcount = 0;

	/// �ؽ�ó�� ���� �������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_DiffuseMapSRV;

	// ��� ���۸� ������� ����
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	DirectX::XMMATRIX _worldTM;
};


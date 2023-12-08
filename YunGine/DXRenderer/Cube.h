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
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) override;
	virtual void Render() override;

private:
	virtual void ObjectSetting() override;

	void BuildFX();
	void BuildVertexLayout();

	void BuildFxFromCso(ID3D11Device* device, const std::wstring& filename);
	void GetTextureFile();

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_textureboxEffect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_textureboxTechnique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_textureboxMatrixVariable;	// ������۸� ����� ����?

	int indexcount = 0;

	/// �ؽ�ó�� ���� �������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_DiffuseMapSRV;
	Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> m_DiffuseMap;
	Microsoft::WRL::ComPtr <ID3DX11EffectMatrixVariable> m_GworldMatrixVariable;
};


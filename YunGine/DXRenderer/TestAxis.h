#pragma once
//����Ʈ �����͸� ����ϱ� ���� ���
#include <wrl\client.h>

// ����Ʈ ���̺귯��
//#pragma comment(lib, "..\\Lib\\Effects11dx64.lib")
//#include "..\\DXEffect\D3dx11effect.h"

#include "d3dx11effect.h"

#include "Vertex.h"
#include <d3d11.h>

class TestAxis
{
public:
	TestAxis(
		Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState);
	~TestAxis();

	void InitTestAxis();

	void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);
	void Render();

private:
	Microsoft::WRL::ComPtr<ID3D11Device>				m_3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_3DDeviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_TestAxisVertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_TestAxisIndexBuffer;		// �ε��� ����	(�������� �ε���?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_TestAxisInputLayout;		// �������� �̰� �˾ƾ� �ұ�?->���⼭ ���ָ�ȴ�.

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_TestAxisEffect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_TestAxisTechnique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> m_TestAxisMatrixVariable;	// ������۸� ����� ����?

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterState;

	void BuildFx();
	void BuildVertexLayout();

	//���ش� ���� �������� �ݵ�� ����ϰ� �ɲ�?
	DirectX::XMMATRIX m_World;	// ���� ��ȯ ��� (���� -> ����)
	DirectX::XMMATRIX m_View;	// �þ� ��ȯ ��� (ī�޶� ��)
	DirectX::XMMATRIX m_Proj;	// ���� ��ȯ ��� (���� / ����)

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

};


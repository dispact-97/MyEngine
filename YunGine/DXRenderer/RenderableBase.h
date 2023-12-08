#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "Vertex.h"
#include "d3dx11effect.h"

// ����Ʈ ���̺귯��
//#pragma comment(lib, "..\\Lib\\Effects11d.lib")
//#include "..\\DXEffect\D3dx11effect.h"

#include <d3dcompiler.h>
#include <d3dcommon.h>

#include "..\\DXTK\Effects.h"


class RenderableBase
{
	// ������Ʈ�� ������ �ʿ�
protected:
	virtual void ObjectSetting() abstract;

	virtual void Update() abstract;
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) abstract;
	virtual void Render() abstract;

	Microsoft::WRL::ComPtr<ID3D11Device>				m_3DDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_3DDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterState;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_VertexBuffer;		// ���� ����		(�������� ����?)
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_IndexBuffer;		// �ε��� ����	(�������� �ε���?)

	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_InputLayout;		// �������� �̰� �˾ƾ� �ұ�?->���⼭ ���ָ�ȴ�.

	Microsoft::WRL::ComPtr<ID3DX11Effect>				m_Effect;			// ���ؽ� ���̴� �ȼ� ���̴��� ��ü�� ����
	Microsoft::WRL::ComPtr<ID3DX11EffectTechnique>		m_Technique;		// ��ũ
	Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable>	m_MatrixVariable;	// ������۸� ����� ����?

	DirectX::XMMATRIX m_World;	// ���� ��ȯ ��� (���� -> ����)
	DirectX::XMMATRIX m_View;	// �þ� ��ȯ ��� (ī�޶� ��)
	DirectX::XMMATRIX m_Proj;	// ���� ��ȯ ��� (���� / ����)

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;

};
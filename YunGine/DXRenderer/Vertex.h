#pragma once
#include <DirectXMath.h> // �������� Math�� ���Խ��� ������

/// <summary>
/// ���ؽ��� �����ǰ� ����� ��ġ������ ��� ���ؽ�
/// </summary>
struct ColorVertex
{
	DirectX::XMFLOAT3 POSTION;		// 0����Ʈ	-> FLOAT�ڷ����� 3���� ��������Ʈ�� 12���� ����
	DirectX::XMFLOAT4 COLOR;		// 12����Ʈ	
};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

// �ؽ�ó�� ���� ���ؽ� �ڷ��� �����
typedef struct UvVertex
{
	DirectX::XMFLOAT3 POSTION;		// 0����Ʈ	-> FLOAT�ڷ����� 3���� ��������Ʈ�� 12���� ����
	DirectX::XMFLOAT2 UV;			// 12����Ʈ ���� -> UV���� 2���� ��ǥ���̶� FLOAT2�� �� �����ϴ�.
}UvVertex;

struct UvNormalVertex
{
	DirectX::XMFLOAT3 position;	// �ҹ��ڷ� �ϴ�����?-> �̸��� ��ġ�� �򰥸� �� �� ����
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texcoord;
};

struct TexVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texc;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};

struct MatrixBufferType					// ��� ����
{
	DirectX::XMMATRIX _world;			// ���� ��ȯ ��� (���� -> ����)
	DirectX::XMMATRIX _view;			// �þ� ��ȯ ��� (ī�޶� ��)
	DirectX::XMMATRIX _projection;		// ���� ��ȯ ��� (���� / ����)
};

struct ConstantBufferData
{
	DirectX::XMMATRIX _worldViewProjection;
};
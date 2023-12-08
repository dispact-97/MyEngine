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
	// �ؽ�ó�� ���µ� ���� �ʿ������� ������ ����� ��ü ǥ�鰣�� ������ ����ϴµ� �ʿ���
	//float3 PosL : POSITION;
	//float3 NormalL : NORMAL;
	//float2 Tex : TEXCOORD;
};

struct TexVertex
{
	DirectX::XMFLOAT3 position;		// 0����Ʈ ������
	//DirectX::XMFLOAT3 normal;		// 12����Ʈ ������
	//DirectX::XMFLOAT3 tangent;		// 24����Ʈ ������
	DirectX::XMFLOAT2 texc;			// 36����Ʈ ������
};

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};


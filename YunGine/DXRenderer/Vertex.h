#pragma once
#include <DirectXMath.h> // 귀찮으니 Math도 포함시켜 버리자

/// <summary>
/// 버텍스의 포지션과 색깔과 위치정보를 담는 버텍스
/// </summary>
struct ColorVertex
{
	DirectX::XMFLOAT3 POSTION;		// 0바이트	-> FLOAT자료형이 3개라서 다음바이트가 12부터 시작
	DirectX::XMFLOAT4 COLOR;		// 12바이트	
};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

// 텍스처를 위한 버텍스 자료형 만들었
typedef struct UvVertex
{
	DirectX::XMFLOAT3 POSTION;		// 0바이트	-> FLOAT자료형이 3개라서 다음바이트가 12부터 시작
	DirectX::XMFLOAT2 UV;			// 12바이트 시작 -> UV값은 2차원 좌표값이라서 FLOAT2가 더 적합하다.
}UvVertex;

struct UvNormalVertex
{
	DirectX::XMFLOAT3 position;	// 소문자로 하는이유?-> 이름이 겹치면 헷갈릴 수 도 있음
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

struct MatrixBufferType					// 상수 버퍼
{
	DirectX::XMMATRIX _world;			// 월드 변환 행렬 (로컬 -> 월드)
	DirectX::XMMATRIX _view;			// 시야 변환 행렬 (카메라 뷰)
	DirectX::XMMATRIX _projection;		// 투영 변환 행렬 (원근 / 직교)
};

struct ConstantBufferData
{
	DirectX::XMMATRIX _worldViewProjection;
};
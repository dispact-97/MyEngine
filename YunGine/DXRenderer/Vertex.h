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
	// 텍스처를 띄우는데 굳이 필요하지는 않지만 조명과 물체 표면간의 각도를 계산하는데 필요함
	//float3 PosL : POSITION;
	//float3 NormalL : NORMAL;
	//float2 Tex : TEXCOORD;
};

struct TexVertex
{
	DirectX::XMFLOAT3 position;		// 0바이트 오프셋
	//DirectX::XMFLOAT3 normal;		// 12바이트 오프셋
	//DirectX::XMFLOAT3 tangent;		// 24바이트 오프셋
	DirectX::XMFLOAT2 texc;			// 36바이트 오프셋
};

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};


////////////////////////////////////////////////////////////////////////////////
// Filename: texture.vs
////////////////////////////////////////////////////////////////////////////////

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

struct VertexInputType
{
	float4 position : POSITIONT;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Vertex Shader

PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;
	
	output.position = mul(input.position, worldMatrix);
	output.position = mul(input.position, viewMatrix);
	output.position = mul(input.position, projectionMatrix);
	
	output.tex = input.tex;
	
	return output;
}
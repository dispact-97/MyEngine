cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

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
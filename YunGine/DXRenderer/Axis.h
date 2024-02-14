#pragma once                                                             

#include "ModelInterface.h"

class Axis final : public ModelInterface
{
public:
	Axis();
	~Axis();

	HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState);
	virtual void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)override;
	virtual void Render() override;
	virtual void Finalzie() override;

	virtual DirectX::XMFLOAT3 GetPosition() override;
	virtual bool SetRenderActive(bool isActive)override;
	virtual bool GetRenderActive()override;

private:
	HRESULT SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState);
	HRESULT SetVertexBuffer();
	HRESULT SetIndexBuffer();
	HRESULT SetConstantBuffer();
	HRESULT SetShader();

	HRESULT CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);

	int indexCount = 0;

	// position
	DirectX::XMFLOAT3 _objectPosition;

	Microsoft::WRL::ComPtr<ID3D11Device> _pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> _pRasterState;		// wire || solid
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer;


	Microsoft::WRL::ComPtr <ID3D11VertexShader> _vertexShader;
	Microsoft::WRL::ComPtr <ID3D11PixelShader> _pixelShader;

	// Transform Matrix
	DirectX::XMMATRIX _world;
	DirectX::XMMATRIX _view;
	DirectX::XMMATRIX _proj;
};

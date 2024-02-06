#pragma once

#include "ModelInterface.h"
#include "ModelBB.h"

class NewCube final : public ModelInterface, public ModelBB
{
public:
	NewCube();
	~NewCube();

	virtual HRESULT Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState) override;
	virtual void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)override;
	virtual void Render() override;
	virtual void Finalzie() override;

	void RotateActive(bool isActive);
	void SetTexture(const char& textureFilePath);
	void Move(float x, float, float z);
	void SetPosition(float x, float y, float z);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 _objectScreenLocation;

	std::vector<DirectX::XMFLOAT3> GetLocalSpaceVertices();
	bool GetRenderActive();

private:
	HRESULT SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr < ID3D11RasterizerState> rasterState);
	HRESULT SetVertexBuffer();
	HRESULT SetIndexBuffer();
	HRESULT SetConstantBuffer();
	HRESULT SetShader();
	HRESULT SetBB();

	HRESULT CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);
	void Calculate2DLocation();

	std::vector<DirectX::XMFLOAT3> _localSpaceVertices;

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

	float _rotationAngle = 1.0f;
	bool _rotateActive;
	bool _renderActive;

	// Transform Matrix
	DirectX::XMMATRIX _world;
	DirectX::XMMATRIX _view;
	DirectX::XMMATRIX _proj;

};
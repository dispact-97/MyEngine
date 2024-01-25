#pragma once                                                             
#include "RenderableBase.h"

class Axis final : public RenderableBase
{
public:
	Axis(
		Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState);
	~Axis();

	virtual void Update() override;
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) override;
	virtual void Render() override;

private:
	virtual void ObjectSetting() override;

	int indexCount = 0;

	// 상수 버퍼를 담기위한 변수
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	HRESULT CreateShader();
	HRESULT CompileShaderFromFile(const wchar_t* filename, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);
	//void CreateShader();

};

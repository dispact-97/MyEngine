#pragma once
#include "RenderableBase.h"

class Grid final : public RenderableBase
{
public:
	Grid(
		Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState);
	~Grid();

	virtual void Update() override;
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) override;
	virtual void Render() override;

private:
	virtual void ObjectSetting() override;

	HRESULT CompileShaderFromFile(const wchar_t* filename,const char* entryPoint, const char* shaderModel,ID3DBlob** blobOut);
	void CreateShader();
	void BuildVertexLayout();

	int indexcount = 0;

};


#pragma once
#include "FbxLoader.h"
#include "RenderableBase.h"

class Model : public RenderableBase
{
public:
	Model(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContext,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterState);
	~Model();

	virtual void Update() override;
	virtual void Render() override;
	virtual void ObjectUpdate(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) override;

	void LoadData(const std::string& filePath);
private:

	bool ObjectSettingSample();

};
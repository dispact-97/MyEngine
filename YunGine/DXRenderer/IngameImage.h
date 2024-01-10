#pragma once
#include "SpriteBatch.h"
#include <string>
#include <d3d11.h>
#include <wrl\client.h>

class RenderableBase;

class IngameImage
{
public:
	//static IngameImage* GetInstance(ID3D11Device* pDevice = nullptr, ID3D11DeviceContext* pDeviceContext = nullptr)
	//{
	//	if (!m_pInstance)
	//	{
	//		m_pInstance = new IngameImage(pDevice, pDeviceContext);
	//	}
	//	return m_pInstance;
	//}
	//IngameImage(const IngameImage&) = delete;
	//IngameImage& operator=(const IngameImage&) = delete;
	//~IngameImage();
	IngameImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~IngameImage();


	void RenderImage(const std::string& Path,const DirectX::XMFLOAT2& position);
	HRESULT InitalizeDXTK();

private:
	//IngameImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSRV;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

	void DrawImage(const DirectX::XMFLOAT2& position);

	static IngameImage* m_pInstance;
};
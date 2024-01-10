#include "IngameImage.h"
#include <iostream>

#include <SimpleMath.h>
#include "RenderableBase.h"
#include "WICTextureLoader.h"

IngameImage* IngameImage::m_pInstance = nullptr;

IngameImage::IngameImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice),
	m_pDeviceContext(pDeviceContext)
{
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(pDeviceContext);

	InitalizeDXTK();
}

HRESULT IngameImage::InitalizeDXTK()
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		m_pDevice,
		m_pDeviceContext,
		L"..\\Resources\\abcd.jpg",
		nullptr,
		m_textureSRV.GetAddressOf());

	if (hr == S_FALSE)
	{
		return hr;
	}

	return hr;
}

IngameImage::~IngameImage()
{

}

void IngameImage::RenderImage(const std::string& Path, const DirectX::XMFLOAT2& position)
{
	DrawImage(position);
}

void IngameImage::DrawImage(const DirectX::XMFLOAT2& position)
{
	m_SpriteBatch->Begin();
	m_SpriteBatch->Draw(m_textureSRV.Get(), position, nullptr, DirectX::Colors::White);
	m_SpriteBatch->End();
}
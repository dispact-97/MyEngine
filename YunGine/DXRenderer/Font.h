#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
#include "SpriteBatch.h"
#include "SpriteFont.h"

class Font
{
public:
	Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~Font();

	void RenderText(const wchar_t* text, float x, float y);

private:
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch = nullptr;
	std::unique_ptr<DirectX::SpriteFont> m_SpriteFont = nullptr;

	ID3D11ShaderResourceView* texture = nullptr;

	HRESULT result;
};


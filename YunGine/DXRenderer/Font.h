#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
#include "..\\DXTK\\SpriteBatch.h"
#include "..\\DXTK\\SpriteFont.h"
class Font
{
public:
	Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~Font();

	void RenderText(const wchar_t* text, float x, float y);

private:
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
	
};


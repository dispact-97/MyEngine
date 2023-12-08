#include "Font.h"

Font::Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(pDeviceContext);
	spriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, L"Arial.spritefont");
}

Font::~Font()
{

}

void Font::RenderText(const wchar_t* text, float x, float y)
{
	DirectX::FXMVECTOR color = DirectX::Colors::White;

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(),text,DirectX::XMFLOAT2(x,y),color);
	spriteBatch->End();
}


#include "Font.h"

#include <iostream>

#include <SimpleMath.h>
#include <DDSTextureLoader.h>
#include "RenderableBase.h"

// ½Ì±ÛÅæ ¸â¹ö º¯¼ö ÃÊ±âÈ­
Font* Font::m_pInstance = nullptr;

Font::Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: result()
{
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(pDeviceContext);
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, L"..\\Font\\NotoSansKR.spritefont");
}

Font::~Font()
{

}

void Font::RenderText(const wchar_t* text, float x, float y)
{
	DirectX::FXMVECTOR color = DirectX::Colors::White;
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), text, DirectX::XMFLOAT2(x, y), color);
	m_SpriteBatch->End();
}

void Font::RenderString(const std::string& str, float x, float y)
{
	DirectX::FXMVECTOR color = DirectX::Colors::White;
	std::wstring wstr(str.begin(), str.end());
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(x, y), color);
	m_SpriteBatch->End();
}

void Font::RenderString(const float floatValue, float x, float y)
{
	DirectX::FXMVECTOR color = DirectX::Colors::White;
	std::string str = std::to_string(floatValue);
	std::wstring wstr(str.begin(), str.end());
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(x, y), color);
	m_SpriteBatch->End();
}

void Font::RenderString(const int intValue, float x, float y)
{
	DirectX::FXMVECTOR color = DirectX::Colors::White;
	std::string str = std::to_string(intValue);
	std::wstring wstr(str.begin(), str.end());
	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(x, y), color);
	m_SpriteBatch->End();
}

void Font::ObjectDebugText(RenderableBase* object)
{
	DirectX::FXMVECTOR color = DirectX::Colors::LightPink;
	//std::string x = std::to_string(object->objectXLocation);
	//std::string y = std::to_string(object->objectYLocation);
	//std::string z = std::to_string(object->objectZLocation);

	std::string x = std::to_string(object->objectPosition.x);
	std::string y = std::to_string(object->objectPosition.y);
	std::string z = std::to_string(object->objectPosition.z);

	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), "Xpos : ", DirectX::XMFLOAT2(object->objectXLocation, object->objectYLocation), color);
	m_SpriteFont->DrawString(m_SpriteBatch.get(), x.c_str(), DirectX::XMFLOAT2(object->objectXLocation + 60, object->objectYLocation), color);
	m_SpriteFont->DrawString(m_SpriteBatch.get(), "Ypos : ", DirectX::XMFLOAT2(object->objectXLocation, object->objectYLocation + 18), color);
	m_SpriteFont->DrawString(m_SpriteBatch.get(), y.c_str(), DirectX::XMFLOAT2(object->objectXLocation + 60, object->objectYLocation + 18), color);
	m_SpriteFont->DrawString(m_SpriteBatch.get(), "Zpos : ", DirectX::XMFLOAT2(object->objectXLocation, object->objectYLocation + 36), color);
	m_SpriteFont->DrawString(m_SpriteBatch.get(), z.c_str(), DirectX::XMFLOAT2(object->objectXLocation + 60, object->objectYLocation + 36), color);
	m_SpriteBatch->End();
}

#include "Font.h"

#include <iostream>

#include <SimpleMath.h>
#include <DDSTextureLoader.h>
#include "RenderableBase.h"

// 싱글톤 멤버 변수 초기화
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
	DirectX::FXMVECTOR color = DirectX::Colors::Crimson;
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

void Font::ObjectDebugText(RenderableBase* object, float width, float height, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	DirectX::FXMVECTOR color = DirectX::Colors::Crimson;

	DirectX::XMVECTOR worldPosition = DirectX::XMVectorSet(object->objectPosition.x, object->objectPosition.y, object->objectPosition.z, 1.0f);
	DirectX::XMVECTOR screenPosition = XMVector3Project(
		worldPosition,
		0,							// 스크린 왼쪽 모서리 x
		0,							// 스크린 왼쪽 모서리 y
		1600,						// 스크린 영역 너비	_windowWidth
		1080,						// 스크린 영역 높이	_windowHeight
		0.0f,						// 깊이 버퍼 최소값
		1.0f,						// 깊이 버퍼 최대값
		projection, view, world);

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

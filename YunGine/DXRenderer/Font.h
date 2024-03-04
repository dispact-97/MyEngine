#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
#include <string>
#include "SpriteBatch.h"
#include "SpriteFont.h"

class RenderableBase;
class NewCube;

class Font
{
public:
	// �̱��� �ν��Ͻ��� �����ϴ� ���� �Լ�
	static Font* GetInstance(ID3D11Device* pDevice = nullptr,ID3D11DeviceContext* pDeviceContext = nullptr)
	{
		// �ν��Ͻ��� ���� ��쿡�� ����
		if (!m_pInstance)
		{
			m_pInstance = new Font(pDevice, pDeviceContext);
		}
		return m_pInstance;
	}

	// ���� �����ڿ� �Ҹ��� ����
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	~Font();

	void RenderText(const wchar_t* text, float x, float y);
	void RenderString(const std::string& str, float x, float y);
	void RenderString(const float floatValue, float x, float y);
	void RenderString(const int intValue, float x, float y);
	void ObjectDebugText(RenderableBase* object);
	void ObjectDebugText(NewCube* object);
	void ObjectDebugText(RenderableBase* object,float width,float height, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);

	// Testing
	void MeasureTextSize(std::string str,float& x, float& y);

private:
	Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;

	HRESULT result;

	static Font* m_pInstance;

	DirectX::XMVECTOR _size;
	float _width = 0.0f;
	float _height = 0.0f;
};

